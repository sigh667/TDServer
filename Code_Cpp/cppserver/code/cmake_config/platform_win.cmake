CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### Platform-specific options
### 平台-特定 选项

# 使用MySQL源文件
option(USE_MYSQL_SOURCES "Use included MySQL-sources to build libraries" 1)

### Package overloads
### 包负荷
# 设置部分库及包含目录
set(ACE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/acelite)
set(ACE_LIBRARY "ace")
set(BZIP2_LIBRARIES "bzip2")
set(ZLIB_LIBRARIES "zlib")

### 使用MySQL时设置MySQL库及包含头文件目录
if( USE_MYSQL_SOURCES )
  set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysqllite/include)
  set(MYSQL_LIBRARY "libmysql")
  set( MYSQL_FOUND 1 )
  message(STATUS "Using supplied MySQL sources")
endif()

# check the CMake preload parameters (commented out by default)
# 检测这个cmake预装载参数

# overload CMAKE_INSTALL_PREFIX if not being set properly
#if( WIN32 )
#  if( NOT CYGWIN )
#    if( NOT CMAKE_INSTALL_PREFIX )
#      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
#    endif()
#  endif()
#endif()

include(${CMAKE_CURRENT_LIST_DIR}/compiler_msvc.cmake)
