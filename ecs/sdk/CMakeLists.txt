cmake_minimum_required(VERSION 3.16)

project(ecs-sdk
    VERSION 0.1.0
    LANGUAGES CXX
)

option(ECS_BUILD_SHARED "Build ECS as shared library" OFF)

add_library(ecs_sdk
    src/cloud_interface.cpp
    src/network.cpp
    src/save_io.cpp
)

set_target_properties(ecs_sdk PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

add_library(ecs::sdk ALIAS ecs_sdk)

target_include_directories(ecs_sdk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(ecs_sdk PUBLIC cxx_std_17)

target_compile_options(ecs_sdk PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
)