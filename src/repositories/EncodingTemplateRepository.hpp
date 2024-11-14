#pragma once

#include <memory>
#include <string>
#include <vector>
#include "database/interfaces/IDatabase.hpp"
#include "dto/EncodingTemplateDTO.hpp"

/**
 * @class EncodingTemplateRepository
 * @brief Repository class for managing encoding templates in a database.
 *
 * This class provides an interface to create, retrieve, update, and delete encoding templates,
 * utilizing the nested structure of EncodingTemplateDTO.
 */
class EncodingTemplateRepository {
public:
    /**
     * @brief Constructor accepting a database instance.
     * @param db A shared pointer to the IDatabase implementation.
     */
    explicit EncodingTemplateRepository(std::shared_ptr<IDatabase> db);

    /**
     * @brief Creates a new encoding template in the database.
     * @param encodingTemplate The EncodingTemplateDTO object containing template data.
     * @return The unique ID of the created template.
     * @throws std::runtime_error if the creation fails.
     */
    [[nodiscard]] int createTemplate(const oatpp::Object<EncodingTemplateDTO> &encodingTemplate) const;

    /**
     * @brief Retrieves an encoding template by its ID.
     * @param templateId The unique ID of the encoding template.
     * @return A shared pointer to EncodingTemplateDTO if found, otherwise nullptr.
     */
    [[nodiscard]] std::shared_ptr<EncodingTemplateDTO> getTemplateById(const std::string& templateId) const;

    /**
     * @brief Retrieves all encoding templates from the database.
     * @return A vector of shared pointers to EncodingTemplateDTO objects.
     */
    [[nodiscard]] std::vector<std::shared_ptr<EncodingTemplateDTO>> getAllTemplates() const;

    /**
     * @brief Updates an existing encoding template in the database.
     * @param templateId The unique ID of the encoding template to update.
     * @param updatedTemplate The updated EncodingTemplateDTO object.
     * @return True if the update was successful, false otherwise.
     */
    [[nodiscard]] bool updateTemplate(const std::string& templateId, const oatpp::Object<EncodingTemplateDTO>& updatedTemplate) const;


    /**
     * @brief Deletes an encoding template by its ID.
     * @param templateId The unique ID of the encoding template to delete.
     * @return True if the deletion was successful, false otherwise.
     */
    [[nodiscard]] bool deleteTemplate(const std::string& templateId) const;

private:
    std::shared_ptr<IDatabase> m_database;

    /**
     * @brief Serializes a SettingsDTO object to a JSON string.
     * @param settings The SettingsDTO object to serialize.
     * @return A JSON string representation of the settings.
     */
    static std::string serializeSettings(const SettingsDTO& settings) ;

    /**
     * @brief Deserializes a JSON string to a SettingsDTO object.
     * @param serializedSettings The JSON string representation of the settings.
     * @return A shared pointer to a SettingsDTO object.
     */
    [[nodiscard]] static std::shared_ptr<SettingsDTO> deserializeSettings(const std::string& serializedSettings) ;

    /**
     * @brief Maps a database row to an EncodingTemplateDTO object.
     * @param row A vector of strings representing a single database row.
     * @return A shared pointer to an EncodingTemplateDTO object.
     */
    [[nodiscard]] static std::shared_ptr<EncodingTemplateDTO> mapToEncodingTemplateDto(const std::vector<std::string>& row) ;
};