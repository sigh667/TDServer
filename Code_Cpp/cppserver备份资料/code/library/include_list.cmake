CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/source/file_list.cmake)

SET(library_include_dir ${CMAKE_CURRENT_LIST_DIR}/source/)
##############################################################
### ◊∞‘ÿ≈‰÷√
##############################################################
# ±È¿˙
foreach(_lib_ ${_Library_List})
	INCLUDE(${CMAKE_CURRENT_LIST_DIR}/source/${_lib_}/file_list.cmake)
endforeach(_lib_)