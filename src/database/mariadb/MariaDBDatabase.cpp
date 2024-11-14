#include "MariaDBDatabase.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>
#include "MariaDBTransaction.hpp"

MariaDBDatabase::MariaDBDatabase(std::string host, std::string user, std::string password,
                                 std::string database, unsigned int port, size_t poolSize)
    : m_host(std::move(host)), m_user(std::move(user)), m_password(std::move(password)),
      m_database(std::move(database)), m_port(port), m_poolSize(poolSize), m_connected(false) {}

MariaDBDatabase::~MariaDBDatabase() {
    if (m_connected) {
        cleanupPool();
        m_connected = false;
        Logger::getInstance().info("MariaDB connection pool closed.");
    }
}

void MariaDBDatabase::connect() {
    if (m_connected) {
        Logger::getInstance().warn("Database is already connected.");
        return;
    }
    try {
        initializePool();
        m_connected = true;
        Logger::getInstance().info("MariaDB connection pool initialized with pool size: " + std::to_string(m_poolSize));
    } catch (const std::runtime_error& e) {
        Logger::getInstance().error("Failed to initialize MariaDB connection pool: " + std::string(e.what()));
        throw;
    }
}

void MariaDBDatabase::disconnect() {
    if (m_connected) {
        cleanupPool();
        m_connected = false;
        Logger::getInstance().info("MariaDB connection pool closed.");
    }
}

bool MariaDBDatabase::isConnected() const {
    return m_connected;
}

int MariaDBDatabase::executeQuery(const std::string& query) {
    return executeQuery(query, {});
}

int MariaDBDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    checkConnection();
    auto connection = acquireConnection();
    int rowsAffected = -1;

    if (params.empty()) {
        if (mysql_query(connection, query.c_str()) == 0) {
            rowsAffected = static_cast<int>(mysql_affected_rows(connection));
            Logger::getInstance().info("Query executed successfully: " + query);
        } else {
            Logger::getInstance().error("MariaDB query failed: " + std::string(mysql_error(connection)));
        }
        releaseConnection(connection);
        return rowsAffected;
    }

    MYSQL_STMT* stmt = mysql_stmt_init(connection);
    if (!stmt || mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        Logger::getInstance().error("Failed to prepare MariaDB statement: " + std::string(mysql_stmt_error(stmt)));
        releaseConnection(connection);
        mysql_stmt_close(stmt);
        return -1;
    }

    std::vector<MYSQL_BIND> bind(params.size());
    for (size_t i = 0; i < params.size(); ++i) {
        bind[i].buffer_type = MYSQL_TYPE_STRING;
        bind[i].buffer = const_cast<void*>(static_cast<const void*>(params[i].c_str()));
        bind[i].buffer_length = params[i].length();
    }

    if (mysql_stmt_bind_param(stmt, bind.data()) != 0 || mysql_stmt_execute(stmt) != 0) {
        Logger::getInstance().error("Failed to execute query with parameters: " + std::string(mysql_stmt_error(stmt)));
    } else {
        rowsAffected = static_cast<int>(mysql_stmt_affected_rows(stmt));
        Logger::getInstance().info("Parameterized query executed successfully: " + query);
    }

    mysql_stmt_close(stmt);
    releaseConnection(connection);
    return rowsAffected;
}

std::vector<std::vector<std::string>> MariaDBDatabase::fetchQuery(const std::string& query) {
    return fetchQuery(query, {});
}

std::vector<std::vector<std::string>> MariaDBDatabase::fetchQuery(const std::string& query, const std::vector<std::string>& params) {
    checkConnection();
    auto connection = acquireConnection();
    std::vector<std::vector<std::string>> rows;

    MYSQL_STMT* stmt = mysql_stmt_init(connection);
    if (!stmt || mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        releaseConnection(connection);
        throw std::runtime_error("Failed to prepare MariaDB statement: " + std::string(mysql_stmt_error(stmt)));
    }

    std::vector<MYSQL_BIND> bind(params.size());
    for (size_t i = 0; i < params.size(); ++i) {
        bind[i].buffer_type = MYSQL_TYPE_STRING;
        bind[i].buffer = const_cast<void*>(static_cast<const void*>(params[i].c_str()));
        bind[i].buffer_length = params[i].length();
    }

    if (!params.empty() && mysql_stmt_bind_param(stmt, bind.data()) != 0) {
        mysql_stmt_close(stmt);
        releaseConnection(connection);
        throw std::runtime_error("Failed to bind parameters: " + std::string(mysql_stmt_error(stmt)));
    }

    if (mysql_stmt_execute(stmt) != 0) {
        mysql_stmt_close(stmt);
        releaseConnection(connection);
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_stmt_error(stmt)));
    }

    MYSQL_RES* metaResult = mysql_stmt_result_metadata(stmt);
    if (!metaResult) {
        mysql_stmt_close(stmt);
        releaseConnection(connection);
        throw std::runtime_error("Failed to retrieve metadata: " + std::string(mysql_stmt_error(stmt)));
    }

    std::vector<MYSQL_BIND> resultBind(mysql_num_fields(metaResult));
    std::vector<char> buffer(1024);
    for (auto& b : resultBind) {
        b.buffer_type = MYSQL_TYPE_STRING;
        b.buffer = buffer.data();
        b.buffer_length = buffer.size();
    }

    mysql_stmt_bind_result(stmt, resultBind.data());
    while (mysql_stmt_fetch(stmt) == 0) {
        std::vector<std::string> row;
        row.reserve(resultBind.size());
        for (auto& b : resultBind) {
            row.emplace_back(static_cast<char*>(b.buffer));
        }
        rows.push_back(row);
    }

    mysql_free_result(metaResult);
    mysql_stmt_close(stmt);
    releaseConnection(connection);
    return rows;
}

std::shared_ptr<ITransaction> MariaDBDatabase::beginTransaction() {
    checkConnection();
    auto connection = acquireConnection();
    return std::make_shared<MariaDBTransaction>(connection);
}

void MariaDBDatabase::checkConnection() const {
    if (!isConnected()) {
        throw std::runtime_error("MariaDBDatabase: Not connected to any database.");
    }
}

void MariaDBDatabase::initializePool() {
    std::lock_guard lock(m_poolMutex);
    for (size_t i = 0; i < m_poolSize; ++i) {
        MYSQL* connection = mysql_init(nullptr);
        if (!connection || !mysql_real_connect(connection, m_host.c_str(), m_user.c_str(), m_password.c_str(),
                                               m_database.c_str(), m_port, nullptr, 0)) {
            throw std::runtime_error("Failed to initialize a MariaDB connection.");
        }
        m_connectionPool.push(connection);
    }
}

MYSQL* MariaDBDatabase::acquireConnection() {
    std::unique_lock lock(m_poolMutex);
    m_condition.wait(lock, [this] { return !m_connectionPool.empty(); });

    MYSQL* connection = m_connectionPool.front();
    m_connectionPool.pop();
    return connection;
}

void MariaDBDatabase::releaseConnection(MYSQL* connection) {
    std::lock_guard lock(m_poolMutex);
    m_connectionPool.push(connection);
    m_condition.notify_one();
}

void MariaDBDatabase::cleanupPool() {
    std::lock_guard lock(m_poolMutex);
    while (!m_connectionPool.empty()) {
        MYSQL* connection = m_connectionPool.front();
        m_connectionPool.pop();
        mysql_close(connection);
    }
}

/**
 * @brief Retrieves the last inserted row ID for the MariaDB database.
 *
 * @return The last inserted row ID as an integer.
 * @throws std::runtime_error if the database is not connected.
 */
int MariaDBDatabase::getLastInsertId() {
    std::lock_guard lock(m_poolMutex);
    checkConnection();

    MYSQL* connection = acquireConnection();
    my_ulonglong lastInsertId = mysql_insert_id(connection);
    releaseConnection(connection);

    if (lastInsertId == 0) {
        throw std::runtime_error("Failed to retrieve last insert ID.");
    }
    return static_cast<int>(lastInsertId);
}

int MariaDBDatabase::executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) {
    // Use parameterized execution logic here
    if (executeQuery(query, params)) {
        auto connection = acquireConnection();
        int lastInsertId = static_cast<int>(mysql_insert_id(connection));
        releaseConnection(connection);
        return lastInsertId;
    }
    return -1;
}