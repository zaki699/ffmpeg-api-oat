#pragma once

#include <memory>
#include <string>
#include <vector>
#include "dto/EncodingDTO.hpp"
#include "repositories/EncodingTemplateRepository.hpp"

/**
 * @class EncodingTemplateManager
 * @brief Manages encoding template operations, leveraging EncodingTemplateRepository for data access.
 */
class EncodingTemplateManager {
public:
    /**
     * @brief Constructor to initialize EncodingTemplateManager with a repository.
     * @param repository The repository handling encoding template database operations.
     */
    explicit EncodingTemplateManager(std::shared_ptr<EncodingTemplateRepository> repository);

    [[nodiscard]] std::string createTemplate(const oatpp::Object<EncodingTemplateDTO>& dto) const;
    [[nodiscard]] oatpp::Object<EncodingTemplateDTO> getTemplate(const std::string& id) const;
    [[nodiscard]] std::vector<oatpp::Object<EncodingTemplateDTO>> getAllTemplates() const;
    [[nodiscard]] bool updateTemplate(const std::string &id, const oatpp::Object<EncodingTemplateDTO>& dto) const;
    [[nodiscard]] bool deleteTemplate(const std::string& id) const;

private:
    std::shared_ptr<EncodingTemplateRepository> m_repository;
};