cmake_minimum_required(VERSION 3.14)
project(YourProjectName)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include FetchContent module
include(FetchContent)

# Set install directories
set(INSTALL_DIR "/usr/local/YourProject")
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")
set(LOG_DIR "${CMAKE_SOURCE_DIR}/logs")

# Output directories for build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Fetching required dependencies
FetchContent_Declare(
        oatpp
        GIT_REPOSITORY https://github.com/oatpp/oatpp.git
        GIT_TAG 1.3.0-latest
)
FetchContent_MakeAvailable(oatpp)

FetchContent_Declare(
        oatpp_sqlite
        GIT_REPOSITORY https://github.com/oatpp/oatpp-sqlite.git
        GIT_TAG 1.3.0-latest
)
FetchContent_MakeAvailable(oatpp_sqlite)

FetchContent_Declare(
        oatpp_swagger
        GIT_REPOSITORY https://github.com/oatpp/oatpp-swagger.git
        GIT_TAG 1.3.0-latest
)
FetchContent_MakeAvailable(oatpp_swagger)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        aws-sdk-cpp
        GIT_REPOSITORY https://github.com/aws/aws-sdk-cpp.git
        GIT_TAG 1.9.100
)
FetchContent_MakeAvailable(aws-sdk-cpp)

# Include directories for headers
include_directories(include)

# Collecting source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries to the project
target_link_libraries(${PROJECT_NAME}
        oatpp::oatpp
        oatpp::oatpp-sqlite
        oatpp::oatpp-swagger
        nlohmann_json::nlohmann_json
        AWS::aws-sdk-cpp
)

# Enable logging (example configuration)
target_compile_definitions(${PROJECT_NAME} PUBLIC OATPP_COMPAT_BUILD_NO_STD_ATOMIC)
add_compile_definitions(LOG_DIR="${LOG_DIR}")
add_compile_definitions(CONFIG_DIR="${CONFIG_DIR}")

# Logging setup for production
set(LOGGING_LEVEL "INFO" CACHE STRING "Set logging level for production (DEBUG, INFO, WARN, ERROR)")

# Installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR}/bin)
install(DIRECTORY ${CONFIG_DIR} DESTINATION ${INSTALL_DIR}/config)
install(DIRECTORY ${LOG_DIR} DESTINATION ${INSTALL_DIR}/logs)

# Define custom log file location
if(NOT EXISTS ${LOG_DIR})
    file(MAKE_DIRECTORY ${LOG_DIR})
endif()

# Define a configuration file setup (example config)
configure_file(${CONFIG_DIR}/app_config.json ${INSTALL_DIR}/config/app_config.json COPYONLY)

# Define compiler flags for optimization in production
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Configuring production build with optimizations.")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()

# Additional configurations if needed
set(AWSSDK_CMAKE_CONFIG_DIR ${aws-sdk-cpp_SOURCE_DIR}/cmake)
set(AWSSDK_ENABLE_S3 ON)
set(BUILD_ONLY "s3")
list(APPEND CMAKE_PREFIX_PATH "${AWSSDK_CMAKE_CONFIG_DIR}")

# Display summary of build configuration
message("Project configured with the following settings:")
message("  - Install directory: ${INSTALL_DIR}")
message("  - Configuration directory: ${CONFIG_DIR}")
message("  - Log directory: ${LOG_DIR}")
message("  - Build type: ${CMAKE_BUILD_TYPE}")
message("  - Logging level: ${LOGGING_LEVEL}")