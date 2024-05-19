cmake_minimum_required(VERSION 3.14)

set(projectName RixEspNetConfig)
set(version 0.0.2)
set(author "Rik Essenius")
set(shortDescription "A library for storing/retrieving network settings using Preferences.")
set(description "This library provides a Configuration class that can store Wifi, MQTT, TLS and other settings in the ESP32 Preferences. It also provides a simple way to retrieve these settings.")
set(category "Data Storage")
set(url https://github/com/essenius/esp32-net-config)
set(architectures esp32)

set(projectTestName ${projectName}Test)
set(espMockName esp32-mock) # lower case is important for fetching the library
set(espMockVersion 0.1.10)
set(safeCstringName safe-cstring)
set(safeCstringVersion 0.6.3)

project(${projectName} VERSION ${version} LANGUAGES CXX)

# We only want to enable testing and code coverage for the top level project
# If we are called from a vcpkg build, we also don't want tests
if ((CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(TOP_LEVEL ON)
else()
  set(TOP_LEVEL OFF)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/lib;${PROJECT_BINARY_DIR}/lib/cmake")

add_subdirectory(lib)
add_subdirectory(src)

if (TOP_LEVEL) 
  message(STATUS "Top level project - enabling tests")
  set(CODE_COVERAGE ON)
  include(code-coverage)
  add_code_coverage()
  enable_testing()
  add_subdirectory(test)
  target_code_coverage(${projectTestName} EXCLUDE build/_deps/*) 
endif()

configure_file(
  library.properties.in
  ${PROJECT_SOURCE_DIR}/library.properties
)

configure_file(
  post-build.cmd.in
  ${PROJECT_SOURCE_DIR}/post-build.cmd
)

configure_file(
  vcpkg.json.in
  ${PROJECT_SOURCE_DIR}/vcpkg.json
)

configure_file(
  README.md.in
  ${PROJECT_SOURCE_DIR}/README.md
)

# to generate coverage report run 'make ccov-${projectTestName}' and view build/ccov/${projectTestName}/index.html