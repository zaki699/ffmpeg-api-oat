#pragma once

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "ConfigManager.hpp"
#include "JobController.hpp"
#include "PluginManager.hpp"

class AppComponent {
public:
    /**
   * Create Async Executor
   */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
        return std::make_shared<oatpp::async::Executor>(
            9 /* Data-Processing threads */,
            2 /* I/O threads */,
            1 /* Timer threads */
        );
    }());

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
        /* non_blocking connections should be used with AsyncHttpConnectionHandler for AsyncIO */
        return oatpp::network::tcp::server::ConnectionProvider::createShared({
            "0.0.0.0", 8000, oatpp::network::Address::IP_4
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
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
        OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
        return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
    }());

    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
        auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
        deserializerConfig->allowUnknownFields = false;
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(
            serializerConfig, deserializerConfig);
        return objectMapper;
    }());

    // PluginManager component
    OATPP_CREATE_COMPONENT(std::shared_ptr<PluginManager>, pluginManager)([] {
        return std::make_shared<PluginManager>();
    }());

    // JobManager component, using dependencies from PluginManager
    OATPP_CREATE_COMPONENT(std::shared_ptr<JobManager>, jobManager)([] {
        OATPP_COMPONENT(std::shared_ptr<PluginManager>, pluginManager);

        auto database = pluginManager->getDatabase();
        auto encodingService = pluginManager->getEncodingService();

        return std::make_shared<JobManager>(database, encodingService);
    }());
};
