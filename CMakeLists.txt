cmake_minimum_required(VERSION 3.14)
project(Analyzer)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include FetchContent module
include(FetchContent)

# Set installation directories
set(INSTALL_DIR "/usr/local/testoat")
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")
set(LOG_DIR "${CMAKE_SOURCE_DIR}/logs")

# Output directories for build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SQLite3 dependency
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

# Manually add the path to the CMake config files
set(CMAKE_PREFIX_PATH "/usr/local/oatpp/lib/cmake")

# Attempt to find and include the oatpp packages
find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-sqlite 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)



# Fetch nlohmann/json if not found
find_package(nlohmann_json CONFIG QUIET)
find_package(AWSSDK REQUIRED COMPONENTS s3)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(crud-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

# Collecting source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/controllers
        ${CMAKE_SOURCE_DIR}/src/interfaces
        ${CMAKE_SOURCE_DIR}/src/dto
        ${CMAKE_SOURCE_DIR}/src/providers
        ${CMAKE_SOURCE_DIR}/src/encoding
        ${CMAKE_SOURCE_DIR}/src/managers
        ${CMAKE_SOURCE_DIR}/src/models
        ${CMAKE_SOURCE_DIR}/src/utils
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/controllers
        ${CMAKE_SOURCE_DIR}/src/interfaces
        ${CMAKE_SOURCE_DIR}/src/providers
        ${CMAKE_SOURCE_DIR}/src/dto
        ${CMAKE_SOURCE_DIR}/src/encoding
        ${CMAKE_SOURCE_DIR}/src/managers
        ${CMAKE_SOURCE_DIR}/src/models
        ${CMAKE_SOURCE_DIR}/src/utils
)

# Link libraries to the project
target_link_libraries(${PROJECT_NAME}
        # Oat++
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite
        nlohmann_json
        aws-sdk-cpp
        ${SQLite3_LIBRARIES}  # Link SQLite3 library
)

# Add configuration and log directories as compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
        CONFIG_DIR="${CONFIG_DIR}"
        LOG_DIR="${LOG_DIR}"
)

# Setting production logging levels and compiler optimizations for release builds
set(LOGGING_LEVEL "INFO" CACHE STRING "Set logging level for production (DEBUG, INFO, WARN, ERROR)")
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Configuring production build with optimizations.")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()

# Ensure log and configuration directories exist
file(MAKE_DIRECTORY ${LOG_DIR})
file(MAKE_DIRECTORY ${CONFIG_DIR})

# Configure the configuration file for deployment
configure_file(${CONFIG_DIR}/config.json ${CMAKE_BINARY_DIR}/config/config.json COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/config/config.json DESTINATION ${INSTALL_DIR}/config)

# Installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR}/bin)
install(DIRECTORY ${CONFIG_DIR} DESTINATION ${INSTALL_DIR}/config)
install(DIRECTORY ${LOG_DIR} DESTINATION ${INSTALL_DIR}/logs)

# Additional AWS SDK configurations for S3 only
set(AWSSDK_CMAKE_CONFIG_DIR ${aws-sdk-cpp_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH "${AWSSDK_CMAKE_CONFIG_DIR}")

# Display build configuration summary
message(STATUS "Project configured with the following settings:")
message(STATUS "  - Install directory: ${INSTALL_DIR}")
message(STATUS "  - Configuration directory: ${CONFIG_DIR}")
message(STATUS "  - Log directory: ${LOG_DIR}")
message(STATUS "  - Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  - Logging level: ${LOGGING_LEVEL}")

# Custom command for build completion message
add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Build completed. Configuration and logs are set up in ${INSTALL_DIR}"
)