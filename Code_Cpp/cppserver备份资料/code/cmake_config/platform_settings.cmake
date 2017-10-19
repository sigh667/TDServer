CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### set installation prefix
### 设置安装前缀
IF( PREFIX )
  SET(CMAKE_INSTALL_PREFIX "${PREFIX}")
ENDIF()

# ${PREFIX}:文件名前缀
# ${SUFFIX}:文件名后缀
# ${CMAKE_CURRENT_LIST_DIR}:当前cmake文件目录

# 设置标准库生成目录
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../bin)

# 装载mysql目录
# INCLUDE(${CMAKE_CURRENT_LIST_DIR}/macros_fnd_mysql.cmake)

##############################################################
### 设置C++编译参数
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