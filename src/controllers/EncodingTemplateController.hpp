#ifndef ENCODINGTEMPLATECONTROLLER_HPP
#define ENCODINGTEMPLATECONTROLLER_HPP

#include "managers/EncodingTemplateManager.hpp"
#include "validation/ValidationUtils.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <memory>
#include <string>
#include <stdexcept>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * @brief Controller for managing encoding templates.
 */
class EncodingTemplateController final : public oatpp::web::server::api::ApiController {
public:
    EncodingTemplateController(const std::shared_ptr<EncodingTemplateManager>& encodingTemplateManager,
                               const std::shared_ptr<ObjectMapper>& objectMapper)
        : oatpp::web::server::api::ApiController(objectMapper)
        , m_encodingTemplateManager(encodingTemplateManager)
    {}

private:
    std::shared_ptr<EncodingTemplateManager> m_encodingTemplateManager;

public:
    /**
     * @brief Endpoint to create a new encoding template.
     */
    ENDPOINT_INFO(createTemplate) {
        info->summary = "Create a new encoding template";
        info->addConsumes<Object<EncodingTemplateDTO>>("application/json");
        info->addResponse<Object<EncodingTemplateDTO>>(Status::CODE_201, "application/json");
        info->addResponse<String>(Status::CODE_400, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "/templates", createTemplate,
             BODY_DTO(Object<EncodingTemplateDTO>, dto)) {
        try {
            Validation::validateEncodingTemplate(dto);
            const std::string newId = m_encodingTemplateManager->createTemplate(dto);
            const auto createdTemplate = m_encodingTemplateManager->getTemplate(newId);
            return createDtoResponse(Status::CODE_201, createdTemplate);
        } catch (const Validation::ValidationException& e) {
            return createResponse(Status::CODE_400, R"({"error":"Validation Error","message":")" + std::string(e.what()) + "\"}");
        } catch (const std::exception& e) {
            return createResponse(Status::CODE_500, R"({"error":"Internal Server Error","message":")" + std::string(e.what()) + "\"}");
        }
    }

    /**
     * @brief Endpoint to retrieve an encoding template by ID.
     */
    ENDPOINT_INFO(getTemplate) {
        info->summary = "Get an encoding template by ID";
        info->pathParams.add("id", oatpp::String::Class::getType()).description = "Template ID";
        info->addResponse<Object<EncodingTemplateDTO>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "/templates/{id}", getTemplate,
             PATH(String, id)) {
        try {
            const auto templateObj = m_encodingTemplateManager->getTemplate(id);
            return createDtoResponse(Status::CODE_200, templateObj);
        } catch (const std::runtime_error& e) {
            return createResponse(Status::CODE_404, R"({"error":"Not Found","message":")" + std::string(e.what()) + "\"}");
        } catch (const std::exception& e) {
            return createResponse(Status::CODE_500, R"({"error":"Internal Server Error","message":")" + std::string(e.what()) + "\"}");
        }
    }

    /**
     * @brief Endpoint to update an existing encoding template.
     */
    ENDPOINT_INFO(updateTemplate) {
        info->summary = "Update an existing encoding template";
        info->pathParams.add("id", oatpp::String::Class::getType()).description = "Template ID";
        info->addConsumes<Object<EncodingTemplateDTO>>("application/json");
        info->addResponse<Object<EncodingTemplateDTO>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_400, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
    }
    ENDPOINT("PUT", "/templates/{id}", updateTemplate,
         PATH(String, id),
         BODY_DTO(Object<EncodingTemplateDTO>, dto)) {
        try {
            Validation::validateEncodingTemplate(dto);

            if (const bool updated = m_encodingTemplateManager->updateTemplate(id, dto); !updated) {
                return createResponse(Status::CODE_404,
                    std::string(R"({"error":"Not Found","message":"Encoding template with ID )") + *id + R"(" not found."})"
                );
            }

            const auto updatedTemplate = m_encodingTemplateManager->getTemplate(id);
            return createDtoResponse(Status::CODE_200, updatedTemplate);
        } catch (const Validation::ValidationException& e) {
            return createResponse(Status::CODE_400, R"({"error":"Validation Error","message":")" + std::string(e.what()) + "\"}");
        } catch (const std::runtime_error& e) {
            return createResponse(Status::CODE_404, R"({"error":"Not Found","message":")" + std::string(e.what()) + "\"}");
        } catch (const std::exception& e) {
            return createResponse(Status::CODE_500, R"({"error":"Internal Server Error","message":")" + std::string(e.what()) + "\"}");
        }
    }

    /**
     * @brief Endpoint to delete an encoding template by ID.
     */
    ENDPOINT_INFO(deleteTemplate) {
        info->summary = "Delete an encoding template by ID";
        info->pathParams.add("id", oatpp::String::Class::getType()).description = "Template ID";
        info->addResponse<String>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_404, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
    }
    ENDPOINT("DELETE", "/templates/{id}", deleteTemplate,
             PATH(String, id)) {
        try {
            if (const bool deleted = m_encodingTemplateManager->deleteTemplate(id); !deleted) {
                return createResponse(Status::CODE_404,
                    std::string(R"({"error":"Not Found","message":"Encoding template with ID )") + *id + R"(" not found."})"
                );
            }
            return createResponse(Status::CODE_200,
                std::string(R"({"message":"Encoding template with ID )") + *id + R"(" deleted successfully."})"
            );
        } catch (const std::exception& e) {
            return createResponse(Status::CODE_500, R"({"error":"Internal Server Error","message":")" + std::string(e.what()) + "\"}");
        }
    }

    /**
     * @brief Endpoint to list all encoding templates.
     */
    ENDPOINT_INFO(listTemplates) {
        info->summary = "List all encoding templates";
        info->addResponse<List<Object<EncodingTemplateDTO>>>(Status::CODE_200, "application/json");
        info->addResponse<String>(Status::CODE_500, "application/json");
    }

    ENDPOINT("GET", "/templates", listTemplates) {
        try {
            // Retrieve templates as a std::vector
            const auto templatesVector = m_encodingTemplateManager->getAllTemplates();

            // Convert std::vector to oatpp::List
            const auto templatesList = oatpp::List<oatpp::Object<EncodingTemplateDTO>>::createShared();
            for (const auto& templateObj : templatesVector) {
                templatesList->push_back(templateObj);
            }

            // Return the list as a response
            return createDtoResponse(Status::CODE_200, templatesList);
        } catch (const std::exception& e) {
            return createResponse(Status::CODE_500, R"({"error":"Internal Server Error","message":")" + std::string(e.what()) + "\"}");
        }
    }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif // ENCODINGTEMPLATECONTROLLER_HPP