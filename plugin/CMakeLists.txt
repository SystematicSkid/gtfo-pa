cmake_minimum_required(VERSION 3.20)

project( gtfo-pa-plugin )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# Glob /src
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Add include directories
include_directories("include")

# Add dll
add_library(${PROJECT_NAME} SHARED ${SOURCES})