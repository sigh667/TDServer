CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)


INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../macro_config.cmake)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../source/map/file_list.cmake)

UNSET(link_lib_list)
LIST(APPEND link_lib_list ${common_link_libraries_list})

# ¹¹½¨¹¤³Ì
LIST(APPEND test_map_source_file_list main.cpp)

macro_add_server_executable(test_map ${CMAKE_CURRENT_LIST_DIR}/../source/map "${link_lib_list}")
