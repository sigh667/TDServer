CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

SET(_name_ "libevent-2.1.8-stable")

# 包含路径
SET(${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR})
IF(WIN32)
	LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/WIN32-Code)
ENDIF()
LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/include)
LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/compat)

#源文件
FILE(GLOB_RECURSE ${_name_}_file_list ${CMAKE_CURRENT_LIST_DIR}/*.h)

#因win与linux引用文件不同，所以需要手动
#公用部分文件
LIST(APPEND ${_name_}_file_list
	# 公共部分
	buffer.c
	bufferevent.c
	bufferevent_filter.c
	bufferevent_pair.c
	bufferevent_ratelim.c
	bufferevent_sock.c
	event.c
	evmap.c
	evthread.c
	evutil.c
	evutil_rand.c
	listener.c
	log.c
	strlcpy.c
	# sys部分
	signal.c
	# extra部分
	evdns.c
	event_tagging.c
	evrpc.c
	http.c	
	)
#win32固有
IF(WIN32)
	LIST(APPEND ${_name_}_file_list
	evthread_win32.c
	win32select.c
	buffer_iocp.c
	event_iocp.c
	bufferevent_async.c
	)
ENDIF()
#linx固有
IF(UNIX)
	LIST(APPEND ${_name_}_file_list
	select.c
	poll.c
	epoll.c
	#linux->pthreads
	evthread_pthread.c
	)
ENDIF()

#其他系统
# devpoll.c
# kqueue.c
# evport.c