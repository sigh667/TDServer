CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

# 配置
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../file_list.cmake)

#获取不同路径名
string(REGEX REPLACE ${game_source_dir}/source/\(.*\) \\1 sub_Library ${CMAKE_CURRENT_LIST_DIR})

macro_add_subLibrary(${sub_Library} "${game_source_dir}")
