#pragma once
#include "database/interfaces/ITransaction.hpp"
#include <mariadb/mysql.h>

class MariaDBTransaction final : public ITransaction {
    MYSQL* m_connection;

public:
    explicit MariaDBTransaction(MYSQL* connection) : m_connection(connection) {
        mysql_query(m_connection, "START TRANSACTION");
    }

    void commit() override {
        mysql_query(m_connection, "COMMIT");
    }

    void rollback() override {
        mysql_query(m_connection, "ROLLBACK");
    }

    ~MariaDBTransaction() override {
        // Ensure transaction is ended properly
        rollback();
    }
};