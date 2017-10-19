################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

 add_compile_options(-std=c++11)

set( CMAKE_USE_RELATIVE_PATHS on)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
#set( RELEASE  $ENV{RELEASE})

        
PROJECT ( gate_client )

set (PRONAME "gate_client")



################ OSBIT #####################
IF(CMAKE_SIZEOF_VOID_P MATCHES 4)
    set ( OSBIT    32)
    MESSAGE(STATUS "OSBIT    32")
ELSE()
    set ( OSBIT 64)
    MESSAGE(STATUS "OSBIT    64")
ENDIF(CMAKE_SIZEOF_VOID_P MATCHES 4)


################ DEBUG/RELEASE #####################
set( CMAKE_BUILD_TYPE Debug )



################ PATH #####################
set(PATH_CUR "./")
set(PATH_CODE "../../code")
set(PATH_BASE "../../code/base")
set(PATH_LOGIC "../../code/logic")
set(PATH_TEST "../../code/test")

IF(WIN32)	
	set(PATH_THIRD_CODE_LIBEVENT "../../third-code/libevent-2.1.8-stable/include")
ELSE()
ENDIF(WIN32)


################ MACRO #####################



################ INCLUDE #####################
include_directories("${PATH_CUR}")
include_directories("${PATH_CODE}")
IF(WIN32)	
	include_directories("${PATH_THIRD_CODE_LIBEVENT}")
ELSE()
ENDIF(WIN32)



################ LIBRARY #####################
IF(WIN32)
	set( LIBS ws2_32  libevent)   
ELSE()
	set( LIBS  
		event
		pthread
		)
ENDIF(WIN32)        


################ LINK_DIRECTORIES #####################
IF(WIN32)
	LINK_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/../../third-libs" )
ELSE()
ENDIF(WIN32)        


################ SOURCE_FILES #####################
AUX_SOURCE_DIRECTORY(${PATH_CUR} SOURCE_FILES)
AUX_SOURCE_DIRECTORY(${PATH_BASE} SOURCE_FILES)
AUX_SOURCE_DIRECTORY(${PATH_LOGIC} SOURCE_FILES)
AUX_SOURCE_DIRECTORY(${PATH_TEST} SOURCE_FILES)


################ HEAD_FILES #####################

MESSAGE( "search headfiles ")

FILE(GLOB_RECURSE  HEAD_FILES_3 "${PATH_CUR}/*.h")
FILE(GLOB_RECURSE  HEAD_FILES_4 "${PATH_BASE}/*.h")
FILE(GLOB_RECURSE  HEAD_FILES_5 "${PATH_LOGIC}/*.h")
FILE(GLOB_RECURSE  HEAD_FILES_6 "${PATH_TEST}/*.h")



set(HEAD_FILES 
		${HEAD_FILES_3}
		${HEAD_FILES_4}
		${HEAD_FILES_5}
		${HEAD_FILES_6}
		)


################ MSVC FILTER #####################


################ CUSTOM COMMANDS #####################


################ BUILD #####################

ADD_EXECUTABLE( ${PRONAME} ${SOURCE_FILES} ${HEAD_FILES} )              
TARGET_LINK_LIBRARIES( ${PRONAME} ${LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PATH_CUR}) 


macro(source_group_by_dir source_files)
endmacro(source_group_by_dir)

set(all_files ${HEAD_FILES} ${SOURCE_FILES})
source_group_by_dir(all_files)


################ INSTALL #####################
