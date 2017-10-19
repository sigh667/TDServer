CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

##############################################################
### 库列表
##############################################################
SET(_Library_List 
	zlib			#zlib
	lua				#lua
	lua_tinker		#lua
	dbMysql
	dbSqlite3
	encrypt
	network
	libevent
	libevent-2.1.8-stable
	json_c
	# jsoncpp
	protobuf
	system
	behaviac
	behaviac3.6
	operators
	json2pb
	recast
	http
	)
##############################################################
### 装载配置
##############################################################
# 遍历
foreach(_lib_ ${_Library_List})
	INCLUDE(${CMAKE_CURRENT_LIST_DIR}/${_lib_}/file_list.cmake)
endforeach(_lib_)
