#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>

/**
 * @brief A class for building SQL queries dynamically.
 *
 * QueryBuilder provides a fluent API for constructing SQL SELECT queries,
 * allowing the user to specify columns, tables, conditions, joins, order, and limits.
 */
class QueryBuilder {
public:
    enum class OrderDirection { ASC, DESC };

    QueryBuilder& select(const std::vector<std::string>& columns);
    QueryBuilder& from(const std::string& table);
    QueryBuilder& where(const std::string& condition, const std::map<std::string, std::string>& parameters = {});
    QueryBuilder& join(const std::string& table, const std::string& condition);
    QueryBuilder& limit(int limit);
    QueryBuilder& orderBy(const std::string& column, OrderDirection direction = OrderDirection::ASC);
    [[nodiscard]] std::string build() const;

private:
    std::vector<std::string> m_selectColumns;
    std::string m_table;
    std::vector<std::string> m_conditions;
    std::vector<std::string> m_joins;
    int m_limit = -1;
    std::string m_orderBy;
    std::map<std::string, std::string> m_parameters;

    static std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
};