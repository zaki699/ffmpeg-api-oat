#include "oatpp/network/Server.hpp"
#include "app/AppComponent.hpp"
#include "controllers/JobController.hpp"
#include "managers/PluginManager.hpp"

void run() {
    AppComponent appComponent;

    auto router = appComponent.router.getObject();
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

    // Register controllers
    auto jobController = JobController::createShared(objectMapper);
    router->addController(jobController);

    // Start server
    oatpp::network::Server server(appComponent.serverConnectionProvider.getObject(), appComponent.httpConnectionHandler.getObject());
    server.run();
}

int main(int argc, const char * argv[]) {
    oatpp::base::Environment::init();

    PluginManager::getInstance().initializePlugins();

    run();

    oatpp::base::Environment::destroy();
    return 0;
}