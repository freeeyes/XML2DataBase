#include "DBOperationCreate.h"


void Create_DB_Environment(_XML_Proc obj_XML_Proc)
{
	char szTempPath[MAX_BUFF_50]   = {'\0'};

	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建LuaIncode目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DBWrapper", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//拷贝指定文件到目录中去
	char szTempFile[MAX_BUFF_100] = {'\0'};
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/conn_pool.h", szTempPath);
	Tranfile("../MysqlCommon/conn_pool.h", szTempFile);
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/conn_pool.cpp", szTempPath);
	Tranfile("../MysqlCommon/conn_pool.cpp", szTempFile);
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/mysql_encap.h", szTempPath);
	Tranfile("../MysqlCommon/mysql_encap.h", szTempFile);
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/mysql_encap.cpp", szTempPath);
	Tranfile("../MysqlCommon/mysql_encap.cpp", szTempFile);
}

bool Create_DB_H(_XML_Proc obj_XML_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.h", 
		obj_XML_Proc.m_sz_ProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "#ifndef _DB_OP_H\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define _DB_OP_H\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"conn_pool.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"mysql_encap.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//需要添加代码

	sprintf_safe(szTemp, 200, "#endif\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
	return true;
}

bool Create_DB_CPP(_XML_Proc obj_XML_Proc)
{
	char szTemp[1024]         = {'\0'};
	char szPathFile[200]      = {'\0'};
	char szSQL[MAX_BUFF_1024] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.cpp", 
		obj_XML_Proc.m_sz_ProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "_XML_Proc obj_XML_Proc \"DB_Op.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//需要添加代码

	fclose(pFile);
	return true;
}

void Create_DB_Proc(_XML_Proc obj_XML_Proc)
{
	Create_DB_Environment(obj_XML_Proc);

	Create_DB_H(obj_XML_Proc);

	Create_DB_CPP(obj_XML_Proc);
}
