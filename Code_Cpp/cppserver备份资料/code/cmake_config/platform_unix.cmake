CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
### Package overloads - Linux
### ������
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(JEMALLOC_LIBRARY "jemalloc")
  message(STATUS "UNIX: Using jemalloc library")
endif()

### set default configuration directory
### ����Ĭ������Ŀ¼
if( NOT CONF_DIR )
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc)
  message(STATUS "UNIX: Using default configuration directory")
endif()

### set default library directory
### ����Ĭ�Ͽ�Ŀ¼
if( NOT LIBSDIR )
  set(LIBSDIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory")
endif()

### configure uninstaller
### ����ж���ļ�
# ${CMAKE_BINARY_DIR}������Ŀ¼��
# ${CMAKE_SOURCE_DIR}��ԴĿ¼��
configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: Configuring uninstall target")

# ����make������ɫ
set(CMAKE_COLOR_MAKEFILE TRUE)

### create uninstaller target (allows for using "make uninstall")
### ����ж��Ŀ��(����ʹ��"make uninstall")
# ${CMAKE_BINARY_DIR}������Ŀ¼��
# # add_custom_target(uninstall
  # # "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
# # )
# # message(STATUS "UNIX: Created uninstall target")

### C����������
if(CMAKE_C_COMPILER MATCHES "gcc")
  include(${CMAKE_CURRENT_LIST_DIR}/compiler_gcc.cmake)
elseif(CMAKE_C_COMPILER MATCHES "icc")
  include(${CMAKE_CURRENT_LIST_DIR}/compiler_icc.cmake)
endif()
