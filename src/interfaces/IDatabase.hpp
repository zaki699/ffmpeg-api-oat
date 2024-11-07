#pragma once

#include <string>
#include <vector>
#include <map>

class IDatabase {
public:
    virtual ~IDatabase() = default;

    // Connection Management
    virtual bool connect(const std::string& connectionString) = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() const = 0;

    // General Query and Execution
    virtual std::vector<std::map<std::string, std::string>> query(const std::string& query) = 0;
    virtual bool execute(const std::string& command) = 0;
};