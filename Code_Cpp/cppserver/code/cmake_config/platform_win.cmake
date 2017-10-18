CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### Platform-specific options
### ƽ̨-�ض� ѡ��

# ʹ��MySQLԴ�ļ�
option(USE_MYSQL_SOURCES "Use included MySQL-sources to build libraries" 1)

### Package overloads
### ������
# ���ò��ֿ⼰����Ŀ¼
set(ACE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/acelite)
set(ACE_LIBRARY "ace")
set(BZIP2_LIBRARIES "bzip2")
set(ZLIB_LIBRARIES "zlib")

### ʹ��MySQLʱ����MySQL�⼰����ͷ�ļ�Ŀ¼
if( USE_MYSQL_SOURCES )
  set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysqllite/include)
  set(MYSQL_LIBRARY "libmysql")
  set( MYSQL_FOUND 1 )
  message(STATUS "Using supplied MySQL sources")
endif()

# check the CMake preload parameters (commented out by default)
# ������cmakeԤװ�ز���

# overload CMAKE_INSTALL_PREFIX if not being set properly
#if( WIN32 )
#  if( NOT CYGWIN )
#    if( NOT CMAKE_INSTALL_PREFIX )
#      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
#    endif()
#  endif()
#endif()

include(${CMAKE_CURRENT_LIST_DIR}/compiler_msvc.cmake)
