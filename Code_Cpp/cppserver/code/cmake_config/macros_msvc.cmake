CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

##############################################################
### 建立虚拟目录(注意:参数使用"${...}")
##############################################################
MACRO(macro_Group_Dir sourceFiles rootDir)
   IF(MSVC)
		#遍历文件
		FOREACH(sgbd_file ${sourceFiles})
			#获取不同路径名
			string(REGEX REPLACE ${rootDir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
			#去掉最后文件名
			string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
			#是否拥空路径
			string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
			#转换路径负号由"/"变为"\"
			string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
			#如果不是空路径构建Group
			IF(NOT sgbd_nogroup)
				SOURCE_GROUP(${macro_Group_Dir_PREFIX}${sgbd_group_name} FILES ${sgbd_file})
			ENDIF()
		ENDFOREACH(sgbd_file)
   ENDIF()
	#取消本次前缀
	UNSET(macro_Group_Dir_PREFIX)
ENDMACRO(macro_Group_Dir)