cmake_minimum_required(VERSION 3.14)
project(Analyzer)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Specify PostgreSQL paths manually for macOS
if(APPLE)
    set(PostgreSQL_ROOT "/opt/homebrew/opt/libpq")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${PostgreSQL_ROOT}")
endif()

# Find PostgreSQL dependency
find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Manually locate MariaDB client library on macOS and Linux
if(APPLE)
    set(MARIADB_INCLUDE_DIR "/opt/homebrew/opt/mariadb-connector-c/include")
    set(MARIADB_LIBRARY "/opt/homebrew/opt/mariadb-connector-c/lib/mariadb/libmariadb.dylib")
elseif(UNIX AND NOT APPLE)
    find_path(MARIADB_INCLUDE_DIR mariadb/mysql.h PATHS /usr/include /usr/local/include /usr/include/mariadb)
    find_library(MARIADB_LIBRARY mariadb PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu /usr/lib64)
endif()

if(NOT MARIADB_LIBRARY OR NOT MARIADB_INCLUDE_DIR)
    message(FATAL_ERROR "Could NOT find MariaDB client library or include directory.")
else()
    message(STATUS "Found MariaDB library: ${MARIADB_LIBRARY}")
    message(STATUS "Found MariaDB include directory: ${MARIADB_INCLUDE_DIR}")
endif()
include_directories(${MARIADB_INCLUDE_DIR})

# Locate oat++ libraries
set(OATPP_ROOT "/usr/local/oatpp/lib/cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${OATPP_ROOT}/oatpp-1.3.0;${OATPP_ROOT}/oatpp-sqlite-1.3.0;${OATPP_ROOT}/oatpp-swagger-1.3.0")

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-sqlite 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)

# Fetch nlohmann/json if not found
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.9.1
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Fetch nlohmann/json-schema-validator if not found
find_package(nlohmann_json_schema_validator CONFIG QUIET)
if(NOT nlohmann_json_schema_validator_FOUND)
    FetchContent_Declare(
            nlohmann_json_schema_validator
            GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
            GIT_TAG 2.3.0
    )
    FetchContent_MakeAvailable(nlohmann_json_schema_validator)
endif()

# Add Abseil dependency if not already included in your CMakeLists
find_package(absl REQUIRED)

# Add Template engine for modern C++
find_package(Inja REQUIRED)

# Fetch Abseil
FetchContent_Declare(
        abseil
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG        20230125.1  # Replace with latest release if needed
)
FetchContent_MakeAvailable(abseil)

# Fetch GoogleTest for testing
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# AWS SDK Configurations
if(APPLE)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}:/opt/homebrew/opt/aws-sdk-cpp")
endif()
find_package(AWSSDK REQUIRED COMPONENTS s3)

find_package(jsoncpp REQUIRED)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Target include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link libraries to the project
target_link_libraries(${PROJECT_NAME}
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite
        nlohmann_json
        nlohmann_json_schema_validator
        jsoncpp_lib
        absl::base
        absl::strings
        absl::log
        inja
        ${SQLite3_LIBRARIES}
        ${AWSSDK_LINK_LIBRARIES}
        ${PostgreSQL_LIBRARY}
        ${MARIADB_LIBRARY}
)

# Link pthread and dynamic linker on Linux
if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} Threads::Threads ${CMAKE_DL_LIBS})
endif()

# Add compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
        CONFIG_DIR="${CONFIG_DIR}"
        LOG_DIR="${LOG_DIR}"
)

# Optimization settings for Release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Configuring production build with optimizations.")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()

# Ensure directories exist
file(MAKE_DIRECTORY ${LOG_DIR})
file(MAKE_DIRECTORY ${CONFIG_DIR})

# Configure deployment
configure_file(${CONFIG_DIR}/config.json ${CMAKE_BINARY_DIR}/config/config.json COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/config/config.json DESTINATION ${INSTALL_DIR}/config)

# Installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR}/bin)
install(DIRECTORY ${CONFIG_DIR} DESTINATION ${INSTALL_DIR}/config)
install(DIRECTORY ${LOG_DIR} DESTINATION ${INSTALL_DIR}/logs)

# Summary message
message(STATUS "Project configured with the following settings:")
message(STATUS "  - Install directory: ${INSTALL_DIR}")
message(STATUS "  - Configuration directory: ${CONFIG_DIR}")
message(STATUS "  - Log directory: ${LOG_DIR}")
message(STATUS "  - Build type: ${CMAKE_BUILD_TYPE}")