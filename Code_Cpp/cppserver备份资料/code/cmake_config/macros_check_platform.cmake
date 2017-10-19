CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### check what platform we're on (64-bit or 32-bit), and create a simpler test than CMAKE_SIZEOF_VOID_P
### 检测当前平台(64位/32位),并创建简单的测试变量CMAKE_SIZEOF_VOID_P

if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
    MESSAGE(STATUS "Detected 64-bit platform")
else()
    set(PLATFORM 32)
    MESSAGE(STATUS "Detected 32-bit platform")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/platform_settings.cmake")

if(WIN32)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_win.cmake")
elseif(APPLE)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_osx.cmake")
elseif(UNIX)
  include("${CMAKE_CURRENT_LIST_DIR}/platform_unix.cmake")
endif()
