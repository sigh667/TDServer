CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

SET(_name_ "dbMysql")

# 包含路径
SET(${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/include)
IF(WIN32)
	LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/win32)
ENDIF()
IF(UNIX)
	LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/unix)
ENDIF()

# 源文件列表
FILE(GLOB_RECURSE _file_list ${CMAKE_CURRENT_LIST_DIR}/include/*.*)
LIST(APPEND ${_name_}_file_list ${_file_list})
FILE(GLOB_RECURSE _file_list ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
LIST(APPEND ${_name_}_file_list ${_file_list})

IF(WIN32)
	FILE(GLOB_RECURSE _file_list ${CMAKE_CURRENT_LIST_DIR}/win32/*.*)
ENDIF()
IF(UNIX)
	FILE(GLOB_RECURSE _file_list ${CMAKE_CURRENT_LIST_DIR}/unix/*.*)
ENDIF()
LIST(APPEND ${_name_}_file_list ${_file_list})

