cmake_minimum_required(VERSION 2.8.3)

IF(UNIX)
#	SET(CMAKE_BUILD_TYPE "Debug") 
#	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#	SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -g -Wall -Wno-multichar -Wno-invalid-offsetof -Wno-non-virtual-dtor -Wno-deprecated -Wunused -std=c++14")
ENDIF()

# 平台设置
include("${CMAKE_CURRENT_LIST_DIR}/platform_settings.cmake")

if(WIN32)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_win.cmake")
elseif(APPLE)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_osx.cmake")
elseif(UNIX)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_unix.cmake")
endif()
