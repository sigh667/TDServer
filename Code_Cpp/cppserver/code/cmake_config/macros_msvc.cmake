CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)

##############################################################
### ��������Ŀ¼(ע��:����ʹ��"${...}")
##############################################################
MACRO(macro_Group_Dir sourceFiles rootDir)
   IF(MSVC)
		#�����ļ�
		FOREACH(sgbd_file ${sourceFiles})
			#��ȡ��ͬ·����
			string(REGEX REPLACE ${rootDir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
			#ȥ������ļ���
			string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
			#�Ƿ�ӵ��·��
			string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
			#ת��·��������"/"��Ϊ"\"
			string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
			#������ǿ�·������Group
			IF(NOT sgbd_nogroup)
				SOURCE_GROUP(${macro_Group_Dir_PREFIX}${sgbd_group_name} FILES ${sgbd_file})
			ENDIF()
		ENDFOREACH(sgbd_file)
   ENDIF()
	#ȡ������ǰ׺
	UNSET(macro_Group_Dir_PREFIX)
ENDMACRO(macro_Group_Dir)