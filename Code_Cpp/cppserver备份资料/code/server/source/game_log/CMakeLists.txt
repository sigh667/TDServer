CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

# 配置
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../file_list.cmake)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)

# 设置标准库生成目录
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../bin)

# 源文件列表
FILE(GLOB_RECURSE source_file_list ${CMAKE_CURRENT_LIST_DIR}/*.*)

# 构建虚拟目录
IF(WIN32)
	macro_Group_Dir("${source_file_list}" "${CMAKE_CURRENT_LIST_DIR}")
ENDIF()
	
# 建立工程
ADD_LIBRARY(game_log ${source_file_list})