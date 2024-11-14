#pragma once

/**
 * @brief Interface for database transactions.
 */
class ITransaction {
public:
    virtual ~ITransaction() = default;

    /**
     * @brief Commits the current transaction.
     * @throws std::runtime_error if commit fails.
     */
    virtual void commit() = 0;

    /**
     * @brief Rolls back the current transaction.
     * @throws std::runtime_error if rollback fails.
     */
    virtual void rollback() = 0;
};