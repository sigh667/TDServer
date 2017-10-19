CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

##############################################################
# �����Ŀ¼�ļ��б�
# _file_list_�������б�
# work_dir������Ŀ¼��
# dir_list��Ŀ¼�б�
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
### �����ӿ��б�
# source_dir��Դ�ļ�Ŀ¼��
# sub_dir_list����Ŀ¼�б�
##############################################################
MACRO(macro_add_sub_directory source_dir sub_dir_list)
	### ��������
	FOREACH(_sub_dir ${sub_dir_list})
		#����ļ�
		ADD_SUBDIRECTORY(${source_dir}${_sub_dir})
	ENDFOREACH(_sub_dir)
ENDMACRO(macro_add_sub_directory)

##############################################################
### �����ӿ��б�
# library_name��������
# root_dir����Ŀ¼��
##############################################################
MACRO(macro_add_subLibrary library_name root_dir)
	# include·�����ļ��б�
	FILE(GLOB_RECURSE _include_list_ "${root_dir}/include/${library_name}/*.*")
	# source·�����ļ��б�
	FILE(GLOB_RECURSE _source_list_ "${root_dir}/source/${library_name}/*.*")

	IF (WIN32)
		# ��������Ŀ¼
		macro_Group_Dir("$_include_list_}" "${root_dir}/include/${library_name}")
		macro_Group_Dir("$_source_list_}" "${root_dir}/source/${library_name}")
	ENDIF()

	#����Դ�б�
	LIST(APPEND _source_list_ ${_include_list_})

	string(REGEX REPLACE ${root_source_dir}/source/\(.*\) \\1 _path_ ${root_dir})
	# ���ÿ����Ŀ¼
	SET(LIBRARY_OUTPUT_PATH ${root_source_dir}/library/${_path_})
		
	# �����⹤��
	ADD_LIBRARY(${library_name} ${_source_list_})
ENDMACRO(macro_add_subLibrary)

##############################################################
### ���ӷ�������ִ�й���
# project_name����Ŀ����
# sub_dir_list����Ŀ¼�б�
# link_libraries_list�����ӿ��б�
##############################################################
MACRO(macro_add_server_executable project_name root_dir link_library_list)
	# 1 ��ȡ��Ŀ¼�ļ��б�
	FILE(GLOB_RECURSE _file_list ${root_dir}/*.*)
	LIST(APPEND ${project_name}_file_list ${_file_list})
	# include��Ŀ¼
	FILE(GLOB_RECURSE _file_list ${root_dir}/include/*.*)
	LIST(APPEND ${project_name}_include_file_list ${_file_list})
	# source��Ŀ¼
	FILE(GLOB_RECURSE _file_list ${root_dir}/source/*.*)
	LIST(APPEND ${project_name}_source_file_list ${_file_list})

	# ��������Ŀ¼
	IF (WIN32)
		SET(macro_Group_Dir_PREFIX "common\\")
		macro_Group_Dir("${common_source_file_list}" ${common_source_dir})
		macro_Group_Dir("${${project_name}_source_list}" "${root_dir}")
		macro_Group_Dir("${${project_name}_include_file_list}" "${root_dir}/include")
		macro_Group_Dir("${${project_name}_source_file_list}" "${root_dir}/source")
	ENDIF()

	### �������ӿ�Ŀ¼
	LINK_DIRECTORIES(${root_dir}/library)

	# ���ÿ����Ŀ¼
	SET(EXECUTABLE_OUTPUT_PATH ${server_output_path}/${project_name})
	# ��������
	ADD_EXECUTABLE(${project_name} ${common_source_file_list}
					#${${project_name}_file_list}
					${${project_name}_include_file_list}
					${${project_name}_source_file_list}
					${${project_name}_source_list})
	# ���ӿ�
	IF(NOT link_library_list)
		TARGET_LINK_LIBRARIES(${project_name} ${link_library_list})
	ENDIF()
ENDMACRO(macro_add_server_executable)