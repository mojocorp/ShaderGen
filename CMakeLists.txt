cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version")

project(ShaderGen)

find_package(Git REQUIRED)

execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --always --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE SHADERGEN_VERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message( STATUS "VERSION: ${SHADERGEN_VERSION}")

add_definitions(-DSHADERGEN_VERSION="${SHADERGEN_VERSION}")

add_subdirectory(source)
