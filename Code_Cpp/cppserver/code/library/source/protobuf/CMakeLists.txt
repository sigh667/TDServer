CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

#装载文件列表
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../macros_build.cmake)

macro_build_library(protobuf ${CMAKE_CURRENT_LIST_DIR} "")