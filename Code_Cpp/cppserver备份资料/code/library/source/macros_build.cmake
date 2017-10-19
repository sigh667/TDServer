CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

##############################################################
### 构建静态库
### project_name = 工程目录名
### project_dir = 工程CmakeLists.txt所在路径【${CMAKE_CURRENT_LIST_DIR}】
##############################################################
MACRO(macro_build_library project_name project_dir include_list)
	# 装载文件列表
	INCLUDE(${project_dir}/../../../cmake_config/config.cmake)
	INCLUDE(${project_dir}/file_list.cmake)
	
	# 遍历附加文件列表
	foreach(_include_ ${include_list})
		INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../${_include_}/file_list.cmake)
		LIST(APPEND ${project_name}_include_dir ${${_include_}_include_dir})
	endforeach(_include_)

	# 包含文件
	INCLUDE_DIRECTORIES(${${project_name}_include_dir})

	# 构建虚拟目录
	IF(WIN32)
		macro_Group_Dir("${${project_name}_file_list}" "${project_dir}")
	ENDIF()

	# 建立工程
	ADD_LIBRARY(${project_name} ${${project_name}_file_list})	
   
ENDMACRO(macro_build_library)