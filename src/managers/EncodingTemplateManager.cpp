#include "EncodingTemplateManager.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>
#include <algorithm>
#include <utils/logger/LoggerMacros.hpp>

/**
 * @brief Constructor to initialize EncodingTemplateManager with a repository.
 * @param repository The repository handling encoding template database operations.
 */
EncodingTemplateManager::EncodingTemplateManager(std::shared_ptr<EncodingTemplateRepository> repository)
    : m_repository(std::move(repository)) {}

/**
 * @brief Creates a new encoding template in the database.
 */
std::string EncodingTemplateManager::createTemplate(const oatpp::Object<EncodingTemplateDTO>& dto) const {
    try {
        const int newId = m_repository->createTemplate(dto);
        Logger::getInstance().info("Created encoding template with ID: " + std::to_string(newId));
        return std::to_string(newId);
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to create encoding template: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief Retrieves an encoding template by its ID.
 */
oatpp::Object<EncodingTemplateDTO> EncodingTemplateManager::getTemplate(const std::string& id) const {
    try {
        auto templateObj = m_repository->getTemplateById(id);
        if (!templateObj) {
            Logger::getInstance().error("Encoding template with ID " + id + " not found.");
            throw std::runtime_error("Encoding template with ID " + id + " not found.");
        }
        Logger::getInstance().info("Retrieved encoding template with ID: " + id);
        return templateObj;
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to retrieve encoding template: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief Retrieves all encoding templates from the database.
 */
std::vector<oatpp::Object<EncodingTemplateDTO>> EncodingTemplateManager::getAllTemplates() const {
    try {
        auto templates = m_repository->getAllTemplates();

        // Convert std::vector<std::shared_ptr<EncodingTemplateDTO>> to std::vector<oatpp::Object<EncodingTemplateDTO>>
        std::vector<oatpp::Object<EncodingTemplateDTO>> oatppTemplates;
        oatppTemplates.reserve(templates.size());

        std::transform(
            templates.begin(),
            templates.end(),
            std::back_inserter(oatppTemplates),
            [](const std::shared_ptr<EncodingTemplateDTO>& ptr) {
                return oatpp::Object<EncodingTemplateDTO>(ptr);  // Convert each shared_ptr to oatpp::Object
            }
        );

        Logger::getInstance().info("Retrieved " + std::to_string(oatppTemplates.size()) + " encoding templates.");
        return oatppTemplates;
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to retrieve encoding templates: %s",std::string(e.what()));
        throw;
    }
}

/**
 * @brief Updates an existing encoding template in the database.
 */
bool EncodingTemplateManager::updateTemplate(const std::string &id, const oatpp::Object<EncodingTemplateDTO>& dto) const {
    try {
        const bool success = m_repository->updateTemplate(id, dto);  // Pass dto directly
        if (success) {
            Logger::getInstance().info("Updated encoding template with ID: " + id);
        } else {
            Logger::getInstance().error("Failed to update encoding template with ID: " + id);
        }
        return success;
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to update encoding template: " + std::string(e.what()));
        throw;
    }
}

/**
 * @brief Deletes an encoding template by its ID.
 */
bool EncodingTemplateManager::deleteTemplate(const std::string& id) const {
    try {
        const bool success = m_repository->deleteTemplate(id);
        if (success) {
            Logger::getInstance().info("Deleted encoding template with ID: " + id);
        } else {
            Logger::getInstance().error("Failed to delete encoding template with ID: " + id);
        }
        return success;
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to delete encoding template: " + std::string(e.what()));
        throw;
    }
}
