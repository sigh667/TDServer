CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists>>>>" ${CMAKE_CURRENT_SOURCE_DIR})
		
# 配置
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/file_list.cmake)
# 游戏日志
LINK_DIRECTORIES(${game_library_dir})

### 设置链接库
UNSET(link_lib_list)
LIST(APPEND link_lib_list ${common_link_libraries_list})

LIST(APPEND map_server_source_file_list main.cpp)

# 构建工程
macro_add_server_executable(map_server ${CMAKE_CURRENT_LIST_DIR} "${link_lib_list}")
