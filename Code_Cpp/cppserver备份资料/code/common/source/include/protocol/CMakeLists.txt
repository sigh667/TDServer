CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

# 配置
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../../../cmake_config/config.cmake)

# 包含目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../library/source/protobuf)


# 设置标准库生成目录
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../library)

# 源文件列表
FILE(GLOB_RECURSE include_file_list ${CMAKE_CURRENT_LIST_DIR}/*.h)
FILE(GLOB_RECURSE source_file_list ${CMAKE_CURRENT_LIST_DIR}/*.cc)

# 建立工程
ADD_LIBRARY(msgprotocol ${source_file_list})