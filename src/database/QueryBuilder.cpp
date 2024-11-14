#include "QueryBuilder.hpp"

QueryBuilder& QueryBuilder::select(const std::vector<std::string>& columns) {
    if (columns.empty()) throw std::invalid_argument("Columns for SELECT cannot be empty");
    m_selectColumns = columns;
    return *this;
}

QueryBuilder& QueryBuilder::from(const std::string& table) {
    if (table.empty()) throw std::invalid_argument("Table name cannot be empty");
    m_table = table;
    return *this;
}

QueryBuilder& QueryBuilder::where(const std::string& condition, const std::map<std::string, std::string>& parameters) {
    if (condition.empty()) throw std::invalid_argument("Condition cannot be empty");
    m_conditions.push_back(condition);
    m_parameters.insert(parameters.begin(), parameters.end());
    return *this;
}

QueryBuilder& QueryBuilder::join(const std::string& table, const std::string& condition) {
    if (table.empty() || condition.empty()) throw std::invalid_argument("JOIN table and condition cannot be empty");
    m_joins.push_back("JOIN " + table + " ON " + condition);
    return *this;
}

QueryBuilder& QueryBuilder::limit(const int limit) {
    if (limit <= 0) throw std::invalid_argument("LIMIT must be a positive integer");
    m_limit = limit;
    return *this;
}

QueryBuilder& QueryBuilder::orderBy(const std::string& column, const OrderDirection direction) {
    if (column.empty()) throw std::invalid_argument("ORDER BY column cannot be empty");
    m_orderBy = column + (direction == OrderDirection::ASC ? " ASC" : " DESC");
    return *this;
}

std::string QueryBuilder::build() const {
    if (m_table.empty()) throw std::logic_error("FROM clause is required");

    std::string query = "SELECT ";
    query += m_selectColumns.empty() ? "*" : join(m_selectColumns, ", ");
    query += " FROM " + m_table;

    for (const auto& join : m_joins) {
        query += " " + join;
    }

    if (!m_conditions.empty()) {
        query += " WHERE " + join(m_conditions, " AND ");
    }

    if (!m_orderBy.empty()) {
        query += " ORDER BY " + m_orderBy;
    }

    if (m_limit > 0) {
        query += " LIMIT " + std::to_string(m_limit);
    }

    return query + ";";
}

std::string QueryBuilder::join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i];
        if (i < elements.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}