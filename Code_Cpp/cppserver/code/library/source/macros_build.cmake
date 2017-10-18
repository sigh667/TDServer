CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

##############################################################
### ������̬��
### project_name = ����Ŀ¼��
### project_dir = ����CmakeLists.txt����·����${CMAKE_CURRENT_LIST_DIR}��
##############################################################
MACRO(macro_build_library project_name project_dir include_list)
	# װ���ļ��б�
	INCLUDE(${project_dir}/../../../cmake_config/config.cmake)
	INCLUDE(${project_dir}/file_list.cmake)
	
	# ���������ļ��б�
	foreach(_include_ ${include_list})
		INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../${_include_}/file_list.cmake)
		LIST(APPEND ${project_name}_include_dir ${${_include_}_include_dir})
	endforeach(_include_)

	# �����ļ�
	INCLUDE_DIRECTORIES(${${project_name}_include_dir})

	# ��������Ŀ¼
	IF(WIN32)
		macro_Group_Dir("${${project_name}_file_list}" "${project_dir}")
	ENDIF()

	# ��������
	ADD_LIBRARY(${project_name} ${${project_name}_file_list})	
   
ENDMACRO(macro_build_library)