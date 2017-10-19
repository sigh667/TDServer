CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

# 配置
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../cmake_config/config.cmake)

# 公共引用文件
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../common/file_list.cmake)

# 包含目录
#INCLUDE_DIRECTORIES(${common_include_dir})
#INCLUDE_DIRECTORIES(${system_include_dir})
#INCLUDE_DIRECTORIES(${behaviac_include_dir})
#INCLUDE_DIRECTORIES(${json2pb_include_dir})
#INCLUDE_DIRECTORIES(${operators_include_dir})
#INCLUDE_DIRECTORIES(${data_include_dir})
#INCLUDE_DIRECTORIES(${library_include_dir})
#INCLUDE_DIRECTORIES(${dbMysql_include_dir})
#INCLUDE_DIRECTORIES(${dbSqlite3_Iinclude_dir})
#INCLUDE_DIRECTORIES(${network_include_dir})
#INCLUDE_DIRECTORIES(${lua_include_dir})
#INCLUDE_DIRECTORIES(${lua_tinker_include_dir})
#INCLUDE_DIRECTORIES(${libevent_include_dir})
INCLUDE_DIRECTORIES(${libevent-2.1.8-stable_include_dir})
#INCLUDE_DIRECTORIES(${zlib_include_dir})
#INCLUDE_DIRECTORIES(${recast_include_dir})
#INCLUDE_DIRECTORIES(${protobuf_include_dir})
#INCLUDE_DIRECTORIES(${http_include_dir})

# 公共链接库列表【引用库顺序最底层放在最后】
SET(common_link_libraries_list libevent)
# lua_functions def_functions msgprotocol zlib dbMysql dbSqlite3 operators network system encrypt json_c lua_tinker lua libcurl http)
# 公共链接库列表
IF(WIN32)
	LIST(APPEND common_link_libraries_list 
				ws2_32.lib
#				libmysql
#				libevent
#				behaviac
#				json2pb
#				json_c
#				recast
#				libprotobuf
				)
ELSEIF(UNIX)
	LIST(APPEND common_link_libraries_list 
				pthread
				rt
				dl
				libevent
				behaviac
				json2pb
				json_c
				recast
				mysqlclient.a
				protobuf
				zlib
				)
ENDIF()
	
# 公共链接库路径
#LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../common/library)

#LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../../lib)
#IF(WIN32)
#	LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../../lib/mysql/win32)
#ENDIF()

#IF(UNIX)
#	LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../../lib/mysql/unix)
#	LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../../lib/dcapi)
#ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/../../bin)

#游戏日志库
#SET(game_log_include_dir ${CMAKE_CURRENT_LIST_DIR}/source/game_log/include)
#SET(collector_include_dir ${CMAKE_CURRENT_LIST_DIR}/source/collector/include)
#SET(game_library_dir ${CMAKE_CURRENT_LIST_DIR}/library)
SET(root_source_dir ${CMAKE_CURRENT_LIST_DIR})

# 设置输出目录
#IF(NOT server_output_path)
#	SET(server_output_path ${CMAKE_CURRENT_LIST_DIR}/../_bin/server)
#ENDIF()
