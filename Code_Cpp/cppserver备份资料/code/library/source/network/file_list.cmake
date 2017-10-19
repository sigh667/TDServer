CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

SET(_name_ "network")

# 包含路径
SET(${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/include)

# 源文件列表
FILE(GLOB_RECURSE ${_name_}_file_list ${CMAKE_CURRENT_LIST_DIR}/*.*)
