#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "utils/ConfigManager.hpp"
#include "controllers/JobController.hpp"
#include "controllers/EncodingTemplateController.hpp"
#include "PluginManager.hpp"
#include "encoding/FFmpegEncodingService.hpp"
#include "repositories/EncodingTemplateRepository.hpp"
#include "repositories/JobRepository.hpp"
#include "managers/EncodingTemplateManager.hpp"
#include "managers/JobManager.hpp"
#include "interfaces/IEncodingService.hpp"
#include "encoding/JobProcessor.hpp"

class AppComponent {
public:
    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    // Get ConfigManager instance
    const auto& config = ConfigManager::getInstance();

    // Fetch IP and port, use defaults if missing
    auto ip = config.get<std::string>("server.ip", "0.0.0.0");  // Default to "0.0.0.0" if not defined
    auto port = config.get<v_uint16>("server.port", 8000);         // Default to 8000 if not defined

    return oatpp::network::tcp::server::ConnectionProvider::createShared({
        ip, port, oatpp::network::Address::IP_4
    });
}());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        const auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
        const auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
        deserializerConfig->allowUnknownFields = false;
        return oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<IEncodingService>, encodingService)([] {
        return std::make_shared<FFmpegEncodingService>();
    }());

    /**
     *  PluginManager component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<PluginManager>, pluginManager)([] {
        auto pluginManager = std::make_shared<PluginManager>();
        pluginManager->initialize();  // Ensure PluginManager initializes plugins
        return pluginManager;
    }());

    /**
     *  EncodingTemplateRepository component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<EncodingTemplateRepository>, encodingTemplateRepository)([] {
        OATPP_COMPONENT(std::shared_ptr<PluginManager>, pluginManager);
        auto database = pluginManager->getDatabase();
        return std::make_shared<EncodingTemplateRepository>(database);
    }());

    /**
     *  EncodingTemplateManager component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<EncodingTemplateManager>, encodingTemplateManager)([] {
        OATPP_COMPONENT(std::shared_ptr<EncodingTemplateRepository>, encodingTemplateRepository);
        return std::make_shared<EncodingTemplateManager>(encodingTemplateRepository);
    }());

    /**
     *  JobRepository component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<JobRepository>, jobRepository)([] {
        OATPP_COMPONENT(std::shared_ptr<PluginManager>, pluginManager);
        auto database = pluginManager->getDatabase();
        return std::make_shared<JobRepository>(database);
    }());

    /**
     *  JobProcessor component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<JobProcessor>, jobProcessor)([] {
        OATPP_COMPONENT(std::shared_ptr<IEncodingService>, encodingService);
        OATPP_COMPONENT(std::shared_ptr<JobRepository>, jobRepository);
        return std::make_shared<JobProcessor>(encodingService, jobRepository);
    }());

    /**
     *  JobManager component, using dependencies from PluginManager and JobProcessor
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<JobManager>, jobManager)([] {
        OATPP_COMPONENT(std::shared_ptr<JobRepository>, jobRepository);
        OATPP_COMPONENT(std::shared_ptr<IEncodingService>, encodingService);
        OATPP_COMPONENT(std::shared_ptr<JobProcessor>, jobProcessor);
        return std::make_shared<JobManager>(jobRepository, encodingService, jobProcessor);
    }());

    /**
     * Register the controllers directly to the router
     */
    static void registerControllers() {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        OATPP_COMPONENT(std::shared_ptr<JobManager>, jobManager);
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper);

        // Create and register JobController
        const auto jobController = std::make_shared<JobController>(jobManager, apiObjectMapper);
        router->addController(jobController);

        // Create and register EncodingTemplateController
        OATPP_COMPONENT(std::shared_ptr<EncodingTemplateManager>, encodingTemplateManager);
        const auto encodingTemplateController = std::make_shared<EncodingTemplateController>(encodingTemplateManager, apiObjectMapper);
        router->addController(encodingTemplateController);
    }
};