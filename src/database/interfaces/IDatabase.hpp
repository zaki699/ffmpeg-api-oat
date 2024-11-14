#pragma once

#include <string>
#include <vector>
#include <condition_variable>
#include "ITransaction.hpp"

/**
 * @brief Interface for database operations, supporting single or pooled connections.
 */
class IDatabase {
public:
    virtual ~IDatabase() = default;

    /**
     * @brief Establishes a connection to the database.
     */
    virtual void connect() = 0;

    /**
     * @brief Closes the connection to the database.
     */
    virtual void disconnect() = 0;

    /**
     * @brief Checks if the database is connected.
     *
     * @return true if connected; otherwise, false.
     */
    [[nodiscard]] virtual bool isConnected() const = 0;

    /**
     * @brief Executes a non-select query (e.g., INSERT, UPDATE, DELETE) with optional parameter binding.
     *
     * This method executes a query that does not return results, such as INSERT, UPDATE, or DELETE.
     * The `params` vector allows binding of values to placeholders (e.g., `?`) in the SQL query,
     * which helps prevent SQL injection and makes the query execution more flexible.
     *
     * @param query The SQL query with optional placeholders for parameterized values.
     * @param params A vector of values to bind to the query's placeholders, in sequential order.
     * @return Number of affected rows if the query was executed successfully, or -1 if it failed.
     */
    virtual int executeQuery(const std::string& query, const std::vector<std::string>& params) = 0;

    /**
     * @brief Executes a non-select query (e.g., INSERT, UPDATE, DELETE) without parameters.
     *
     * @param query The SQL query to execute.
     * @return Number of affected rows if the query was executed successfully, or -1 if it failed.
     */
    virtual int executeQuery(const std::string& query) = 0;

    /**
     * @brief Executes a select query and fetches results.
     *
     * This method fetches data in response to a SELECT query. Use this function
     * to retrieve results without binding parameters.
     *
     * @param query The SQL query.
     * @return Query results as a vector of rows, each row a vector of column values.
     */
    virtual std::vector<std::vector<std::string>> fetchQuery(const std::string& query) = 0;

    /**
     * @brief Executes a select query with parameters and fetches results.
     *
     * This method fetches data in response to a SELECT query with parameters,
     * which allows safe binding of user-provided values.
     *
     * @param query The SQL query with placeholders for parameters.
     * @param params A vector of values to bind to the query's placeholders, in sequential order.
     * @return Query results as a vector of rows, each row a vector of column values.
     */
    virtual std::vector<std::vector<std::string>> fetchQuery(const std::string& query, const std::vector<std::string>& params) = 0;

    /**
     * @brief Begins a transaction.
     *
     * @return A transaction object.
     */
    virtual std::shared_ptr<ITransaction> beginTransaction() = 0;


    /**
     * @brief Retrieves the last inserted row ID.
     *
     * @return The last inserted row ID as an integer.
     * @throws std::runtime_error if the ID cannot be retrieved.
     */
    virtual int getLastInsertId() = 0;

    /**
    * @brief Executes an INSERT SQL query and retrieves the last inserted ID.
    *
    * @param query The SQL query with optional placeholders for parameterized values.
    * @param params A vector of values to bind to the query's placeholders, in sequential order.
    * @return The last inserted row ID, or -1 if not applicable or an error occurs.
    */
    virtual int executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) = 0;
};