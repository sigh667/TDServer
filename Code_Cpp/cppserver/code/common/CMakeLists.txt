CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

MESSAGE(STATUS ">>>>CMakeLists:" ${CMAKE_CURRENT_SOURCE_DIR})

# ≈‰÷√
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../cmake_config/config.cmake)

# include
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/file_list.cmake)

### π§≥Ã√˚
PROJECT(common)
# 
ADD_SUBDIRECTORY(source)


