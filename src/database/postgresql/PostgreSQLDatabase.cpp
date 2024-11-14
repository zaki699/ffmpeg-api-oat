#include "PostgreSQLDatabase.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>

PostgreSQLDatabase::PostgreSQLDatabase(std::string conninfo, const size_t poolSize)
    : m_conninfo(std::move(conninfo)), m_poolSize(poolSize), m_connected(false) {}

PostgreSQLDatabase::~PostgreSQLDatabase() {
    if (m_connected) {
        cleanupPool();
        m_connected = false;
        Logger::getInstance().info("PostgreSQL connection pool closed.");
    }
}

void PostgreSQLDatabase::connect() {
    if (m_connected) {
        Logger::getInstance().warn("Database is already connected.");
        return;
    }

    try {
        initializePool();
        m_connected = true;
        Logger::getInstance().info("PostgreSQL connection pool initialized with pool size: " + std::to_string(m_poolSize));
    } catch (const std::runtime_error& e) {
        Logger::getInstance().error("Failed to initialize PostgreSQL connection pool: " + std::string(e.what()));
        throw;
    }
}

void PostgreSQLDatabase::disconnect() {
    if (m_connected) {
        cleanupPool();
        m_connected = false;
        Logger::getInstance().info("PostgreSQL connection pool closed.");
    }
}

bool PostgreSQLDatabase::isConnected() const {
    return m_connected;
}

int PostgreSQLDatabase::executeQuery(const std::string& query) {
    return executeQuery(query, {});
}

int PostgreSQLDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    checkConnection();

    const auto conn = acquireConnection();
    std::vector<const char*> c_params;
    c_params.reserve(params.size());
    for (const auto& param : params) {
        c_params.push_back(param.c_str());
    }

    PGresult* result = PQexecParams(conn,
                                    query.c_str(),
                                    static_cast<int>(params.size()),
                                    nullptr,
                                    c_params.data(),
                                    nullptr,
                                    nullptr,
                                    0);

    int affectedRows = -1;
    if (PQresultStatus(result) == PGRES_COMMAND_OK) {
        affectedRows = std::stoi(PQcmdTuples(result));
        Logger::getInstance().info("Parameterized query executed successfully: " + query);
    } else {
        Logger::getInstance().error("PostgreSQLDatabase query execution failed: " + std::string(PQerrorMessage(conn)));
    }

    PQclear(result);
    releaseConnection(conn);
    return affectedRows;
}

std::vector<std::vector<std::string>> PostgreSQLDatabase::fetchQuery(const std::string& query) {
    return fetchQuery(query, {});
}

std::vector<std::vector<std::string>> PostgreSQLDatabase::fetchQuery(const std::string& query, const std::vector<std::string>& params) {
    checkConnection();
    const auto conn = acquireConnection();

    std::vector<const char*> c_params;
    c_params.reserve(params.size());
    for (const auto& param : params) {
        c_params.push_back(param.c_str());
    }

    PGresult* result = PQexecParams(conn,
                                    query.c_str(),
                                    static_cast<int>(params.size()),
                                    nullptr,
                                    c_params.data(),
                                    nullptr,
                                    nullptr,
                                    0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        const std::string error = PQerrorMessage(conn);
        PQclear(result);
        releaseConnection(conn);
        throw std::runtime_error("PostgreSQLDatabase query fetch failed: " + error);
    }

    const int numRows = PQntuples(result);
    const int numCols = PQnfields(result);
    std::vector<std::vector<std::string>> rows;
    rows.reserve(numRows);

    for (int i = 0; i < numRows; ++i) {
        std::vector<std::string> row;
        row.reserve(numCols);
        for (int j = 0; j < numCols; ++j) {
            row.emplace_back(PQgetvalue(result, i, j));
        }
        rows.emplace_back(std::move(row));
    }

    PQclear(result);
    releaseConnection(conn);
    return rows;
}

std::shared_ptr<ITransaction> PostgreSQLDatabase::beginTransaction() {
    checkConnection();
    executeQuery("BEGIN");
    return nullptr;
}

void PostgreSQLDatabase::initializePool() {
    std::lock_guard<std::mutex> lock(m_poolMutex);
    for (size_t i = 0; i < m_poolSize; ++i) {
        PGconn* conn = PQconnectdb(m_conninfo.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            const std::string error = PQerrorMessage(conn);
            PQfinish(conn);
            throw std::runtime_error("Failed to initialize PostgreSQL connection: " + error);
        }
        m_connectionPool.push(conn);
    }
}

PGconn* PostgreSQLDatabase::acquireConnection() {
    std::unique_lock<std::mutex> lock(m_poolMutex);
    m_condition.wait(lock, [this] { return !m_connectionPool.empty(); });

    PGconn* conn = m_connectionPool.front();
    m_connectionPool.pop();
    return conn;
}

void PostgreSQLDatabase::releaseConnection(PGconn* conn) {
    std::lock_guard<std::mutex> lock(m_poolMutex);
    m_connectionPool.push(conn);
    m_condition.notify_one();
}

void PostgreSQLDatabase::checkConnection() const {
    if (!m_connected) {
        throw std::runtime_error("PostgreSQLDatabase: Not connected to any database.");
    }
}

void PostgreSQLDatabase::cleanupPool() {
    std::lock_guard<std::mutex> lock(m_poolMutex);
    while (!m_connectionPool.empty()) {
        PGconn* conn = m_connectionPool.front();
        m_connectionPool.pop();
        PQfinish(conn);
    }
}

int PostgreSQLDatabase::getLastInsertId() {
    throw std::runtime_error("PostgreSQLDatabase: getLastInsertId not available on PostgreSQL");
}

int PostgreSQLDatabase::executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) {
    // Use the PostgreSQL `RETURNING` clause in the query, and fetch the result.
    auto result = fetchQuery(query, params);
    if (!result.empty() && !result[0].empty()) {
        return std::stoi(result[0][0]);
    }
    return -1;
}
