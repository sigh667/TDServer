CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

##############################################################
# 获得子目录文件列表
# _file_list_【返回列表】
# work_dir【工作目录】
# dir_list【目录列表】
##############################################################
MACRO(macro_get_sub_dir_file_list _file_list_ work_dir dir_list)

	FOREACH(_dir_ ${dir_list})
		FILE(GLOB_RECURSE _file_list ${work_dir}/include/${_dir_}/*.*)
		LIST(APPEND _file_list_ _file_list)
		
		FILE(GLOB_RECURSE _file_list ${work_dir}/source/${_dir_}/*.*)
		LIST(APPEND _file_list_ _file_list)
		
	ENDFOREACH(_dir_)

ENDMACRO(macro_get_sub_dir_file_list)

##############################################################
### 包含子库列表
# source_dir【源文件目录】
# sub_dir_list【子目录列表】
##############################################################
MACRO(macro_add_sub_directory source_dir sub_dir_list)
	### 包含工程
	FOREACH(_sub_dir ${sub_dir_list})
		#获得文件
		ADD_SUBDIRECTORY(${source_dir}${_sub_dir})
	ENDFOREACH(_sub_dir)
ENDMACRO(macro_add_sub_directory)

##############################################################
### 包含子库列表
# library_name【库名】
# root_dir【根目录】
##############################################################
MACRO(macro_add_subLibrary library_name root_dir)
	# include路径及文件列表
	FILE(GLOB_RECURSE _include_list_ "${root_dir}/include/${library_name}/*.*")
	# source路径及文件列表
	FILE(GLOB_RECURSE _source_list_ "${root_dir}/source/${library_name}/*.*")

	IF (WIN32)
		# 构建虚拟目录
		macro_Group_Dir("$_include_list_}" "${root_dir}/include/${library_name}")
		macro_Group_Dir("$_source_list_}" "${root_dir}/source/${library_name}")
	ENDIF()

	#设置源列表
	LIST(APPEND _source_list_ ${_include_list_})

	string(REGEX REPLACE ${root_source_dir}/source/\(.*\) \\1 _path_ ${root_dir})
	# 设置库输出目录
	SET(LIBRARY_OUTPUT_PATH ${root_source_dir}/library/${_path_})
		
	# 建立库工程
	ADD_LIBRARY(${library_name} ${_source_list_})
ENDMACRO(macro_add_subLibrary)

##############################################################
### 增加服务器可执行工程
# project_name【项目名】
# sub_dir_list【子目录列表】
# link_libraries_list【链接库列表】
##############################################################
MACRO(macro_add_server_executable project_name root_dir link_library_list)
	# 1 获取根目录文件列表
	FILE(GLOB_RECURSE _file_list ${root_dir}/*.*)
	LIST(APPEND ${project_name}_file_list ${_file_list})
	# include子目录
	FILE(GLOB_RECURSE _file_list ${root_dir}/include/*.*)
	LIST(APPEND ${project_name}_include_file_list ${_file_list})
	# source子目录
	FILE(GLOB_RECURSE _file_list ${root_dir}/source/*.*)
	LIST(APPEND ${project_name}_source_file_list ${_file_list})

	# 构建虚拟目录
	IF (WIN32)
		SET(macro_Group_Dir_PREFIX "common\\")
		macro_Group_Dir("${common_source_file_list}" ${common_source_dir})
		macro_Group_Dir("${${project_name}_source_list}" "${root_dir}")
		macro_Group_Dir("${${project_name}_include_file_list}" "${root_dir}/include")
		macro_Group_Dir("${${project_name}_source_file_list}" "${root_dir}/source")
	ENDIF()

	### 设置链接库目录
	LINK_DIRECTORIES(${root_dir}/library)

	# 设置库输出目录
	SET(EXECUTABLE_OUTPUT_PATH ${server_output_path}/${project_name})
	# 建立工程
	ADD_EXECUTABLE(${project_name} ${common_source_file_list}
					#${${project_name}_file_list}
					${${project_name}_include_file_list}
					${${project_name}_source_file_list}
					${${project_name}_source_list})
	# 链接库
	IF(NOT link_library_list)
		TARGET_LINK_LIBRARIES(${project_name} ${link_library_list})
	ENDIF()
ENDMACRO(macro_add_server_executable)