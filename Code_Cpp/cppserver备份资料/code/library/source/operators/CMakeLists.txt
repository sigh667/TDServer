CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

# 装载文件列表
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../system/file_list.cmake)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../macros_build.cmake)

INCLUDE_DIRECTORIES(${library_include_dir})
INCLUDE_DIRECTORIES(${lua_tinker_include_dir})
INCLUDE_DIRECTORIES(${lua_include_dir})
INCLUDE_DIRECTORIES(${libevent_include_dir})
INCLUDE_DIRECTORIES(${system_include_dir})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/..)

macro_build_library(operators ${CMAKE_CURRENT_LIST_DIR} "${include_libraries_list}")