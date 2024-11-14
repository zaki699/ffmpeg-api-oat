#include "oatpp/network/Server.hpp"
#include "app/AppComponent.hpp"
#include "controllers/JobController.hpp"
#include "app/PluginManager.hpp"
#include "utils/logger/LoggerMacros.hpp"
#include "utils/logger/FileLogSink.hpp"
#include <iostream>

#include "absl/log/initialize.h"

void run() {
    // Initialize AppComponent, which includes all dependency injection components
    AppComponent appComponent;

    // Register controllers using the AppComponent's registerControllers method
    AppComponent::registerControllers();

    // Start server with configured connection provider and handler
    oatpp::network::Server server(
        appComponent.serverConnectionProvider.getObject(),
        appComponent.serverConnectionHandler.getObject()
    );

    // Log server start info
    OATPP_LOGD("Server", "Running on port %s...",
               appComponent.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());

    // Run the server (blocking call)
    server.run();
}

int main(int argc, const char * argv[]) {
    // Initialize Oat++ Environment
    oatpp::base::Environment::init();

    // Initialize logging to file
    const auto fileSink = std::make_unique<FileLogSink>("app.log");
    absl::AddLogSink(fileSink.get());

    // Example usage of the logger
    LOG_INFO("Starting the application...");

    // Run the main application
    run();

    // Report environment information for debugging
    LOG_INFO("objectsCount = %d",oatpp::base::Environment::getObjectsCount());
    LOG_INFO("objectsCreated = %d",oatpp::base::Environment::getObjectsCreated());

    // Destroy Oat++ Environment
    oatpp::base::Environment::destroy();

    // Remember to remove sink when no longer needed
    absl::RemoveLogSink(fileSink.get());

    return 0;
}