CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

SET(_name_ "dbSqlite3")

# ����·��
SET(${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/include)
LIST(APPEND ${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/include/include)

# Դ�ļ��б�
FILE(GLOB_RECURSE ${_name_}_file_list ${CMAKE_CURRENT_LIST_DIR}/*.*)