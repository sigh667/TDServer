CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### set up output paths for executable binaries (.exe-files, and .dll-files on DLL-capable platforms)
### 设置可执行二进制文件输出路径
# ${CMAKE_BINARY_DIR}:输出二进制目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_CURRENT_LIST_DIR}/macros_msvc.cmake)

### set up output paths ofr static libraries etc (commented out - shown here as an example only)
### 设置静态库输出路径等
###set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
###set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 路径设置
SET(CMAKE_USE_RELATIVE_PATHS true)		#使用相对路径
SET(CMAKE_SUPPRESS_REGENERATION true)	#不生成ZERO_CHECK工程

if(PLATFORM EQUAL 64)
  # This definition is necessary to work around a bug with Intellisense described
  # here: http://tinyurl.com/2cb428.  Syntax highlighting is important for proper
  # debugger functionality.
  ### 必要的增加设置
  #64位系统
  add_definitions("-D_WIN64")
  message(STATUS "MSVC: 64-bit platform, enforced -D_WIN64 parameter")

  #Enable extended object support for debug compiles on X64 (not required on X86)
  #能够扩展对像支持调试编译到64位
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
  message(STATUS "MSVC: Enabled extended object-support for debug-compiles")
else()
  # mark 32 bit executables large address aware so they can use > 2GB address space
  # 标记的32位可执行文件大地址访问，他们可以使用>2GB内存地址空间
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
  message(STATUS "MSVC: Enabled large address awareness")

  # Test if we need SSE2-support
  if(USE_SFMT)
    add_definitions(/arch:SSE2)
    message(STATUS "MSVC: Enabled SSE2 support")
  endif()
endif()

### Set build-directive (used in core to tell which buildtype we used)
### 设置构建目录(使用核心断定使用的构建类型)
# add_definitions(-D_BUILD_DIRECTIVE=\\"$(ConfigurationName)\\")

### multithreaded compiling on VS
### 多线程编译
### set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

### Define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES - eliminates the warning by changing the strcpy call to strcpy_s, which prevents buffer overruns
### 定义 _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES -排除警告strcpy调用strcpy_s,放置缓冲益处
add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
message(STATUS "MSVC: Overloaded standard names")

### Ignore warnings about older, less secure functions
### 忽略关于旧的井盖,不安全的功能
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
message(STATUS "MSVC: Disabled NON-SECURE warnings")

# Ignore warnings about POSIX deprecation
add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
message(STATUS "MSVC: Disabled POSIX warnings")

### disable warnings in Visual Studio 8 and above if not wanted
### 禁用警告
# CMAKE_GENERATOR: 编译器名称
# if(NOT WITH_WARNINGS)
  # if(MSVC AND NOT CMAKE_GENERATOR MATCHES "Visual Studio 7")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996 /wd4355 /wd4244 /wd4985 /wd4267 /wd4619")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4355 /wd4244 /wd4985 /wd4267 /wd4619")
    # message(STATUS "MSVC: Disabled generic compiletime warnings")
  # endif()
# endif()
