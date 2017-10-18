CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### Package overloads - Linux
### 包负荷
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(JEMALLOC_LIBRARY "jemalloc")
  message(STATUS "UNIX: Using jemalloc library")
endif()

### set default configuration directory
### 设置默认配置目录
if( NOT CONF_DIR )
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc)
  message(STATUS "UNIX: Using default configuration directory")
endif()

### set default library directory
### 设置默认库目录
if( NOT LIBSDIR )
  set(LIBSDIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory")
endif()

### configure uninstaller
### 配置卸载文件
# ${CMAKE_BINARY_DIR}【生成目录】
# ${CMAKE_SOURCE_DIR}【源目录】
configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: Configuring uninstall target")

# 设置make生成颜色
set(CMAKE_COLOR_MAKEFILE TRUE)

### create uninstaller target (allows for using "make uninstall")
### 创建卸载目标(允许使用"make uninstall")
# ${CMAKE_BINARY_DIR}【生成目录】
# # add_custom_target(uninstall
  # # "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
# # )
# # message(STATUS "UNIX: Created uninstall target")

### C编译器配置
if(CMAKE_C_COMPILER MATCHES "gcc")
  include(${CMAKE_CURRENT_LIST_DIR}/compiler_gcc.cmake)
elseif(CMAKE_C_COMPILER MATCHES "icc")
  include(${CMAKE_CURRENT_LIST_DIR}/compiler_icc.cmake)
endif()
