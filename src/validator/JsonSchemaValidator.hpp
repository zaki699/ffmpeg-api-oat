#ifndef JSON_SCHEMA_VALIDATOR_HPP
#define JSON_SCHEMA_VALIDATOR_HPP

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/Types.hpp>


class JsonSchemaValidator {
private:
    nlohmann::json_schema::json_validator validator;
    nlohmann::json schema;

public:
    // Constructor to initialize with a schema string
    explicit JsonSchemaValidator(const std::string& schemaStr);

    // Validate JSON data from Oat++ DTO
    bool validate(const oatpp::Object<oatpp::DTO>& dto, const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper, std::string& errorMsg) const;

    // Load schema from a file
    static nlohmann::json loadSchemaFromFile(const std::string& filePath);
};

#endif // JSON_SCHEMA_VALIDATOR_HPP