CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### set installation prefix
### ���ð�װǰ׺
IF( PREFIX )
  SET(CMAKE_INSTALL_PREFIX "${PREFIX}")
ENDIF()

# ${PREFIX}:�ļ���ǰ׺
# ${SUFFIX}:�ļ�����׺
# ${CMAKE_CURRENT_LIST_DIR}:��ǰcmake�ļ�Ŀ¼

# ���ñ�׼������Ŀ¼
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../bin)

# װ��mysqlĿ¼
# INCLUDE(${CMAKE_CURRENT_LIST_DIR}/macros_fnd_mysql.cmake)

##############################################################
### ����C++�������
##############################################################
IF(WIN32)
	FOREACH(flag_var
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELWITHDEBINFO)

		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MD")
		if(${flag_var} MATCHES "/MDd")
			string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MDd")
	ENDFOREACH(flag_var)
ENDIF()