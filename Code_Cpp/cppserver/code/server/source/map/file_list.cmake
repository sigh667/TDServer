CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../macro_config.cmake)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../file_list.cmake)

IF(NOT game_config)
	SET(game_config TRUE)
	SET(game_source_dir ${CMAKE_CURRENT_LIST_DIR})
	# 服务器目录
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)
	INCLUDE_DIRECTORIES(${game_log_include_dir})
	INCLUDE_DIRECTORIES(${collector_include_dir})
	# 链接库目录
	LINK_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../library)
ENDIF()
