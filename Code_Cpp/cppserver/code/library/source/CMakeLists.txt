CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../cmake_config/config.cmake)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/file_list.cmake)

# 添加library源目录
foreach(ProjectName ${_Library_List})
	#添加源目录
	ADD_SUBDIRECTORY(${ProjectName})
endforeach(ProjectName)