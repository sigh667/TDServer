CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

SET(_name_ "http")

# ����·��
SET(${_name_}_include_dir ${CMAKE_CURRENT_LIST_DIR}/curl_cpp)

# Դ�ļ��б�
FILE(GLOB_RECURSE ${_name_}_file_list ${CMAKE_CURRENT_LIST_DIR}/curl_cpp/*.*)
