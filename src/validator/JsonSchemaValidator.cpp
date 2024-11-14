#include "JsonSchemaValidator.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

JsonSchemaValidator::JsonSchemaValidator(const std::string& schemaStr) {
    try {
        schema = json::parse(schemaStr);
        validator.set_root_schema(schema);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to initialize schema: " + std::string(e.what()));
    }
}

json JsonSchemaValidator::loadSchemaFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open schema file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return json::parse(buffer.str());
}

bool JsonSchemaValidator::validate(const oatpp::Object<oatpp::DTO>& dto, const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper, std::string& errorMsg) const {
    try {
        // Serialize DTO to JSON using Oat++ ObjectMapper
        const auto jsonString = objectMapper->writeToString(dto);
        const json data = json::parse(jsonString->c_str());

        // Perform validation (throws exception if validation fails)
        validator.validate(data);

        // If no exception is thrown, validation is successful
        return true;

    } catch (const std::exception& e) {
        errorMsg = "Validation failed: " + std::string(e.what());
    }

    return false;
}
