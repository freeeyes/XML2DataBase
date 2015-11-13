#include "DBOperationCreate.h"

bool Create_Insert_SQL(_DB_Table& obj_DB_Table, char* pText)
{
	char szTemp[MAX_BUFF_500] = {'\0'};
	char szTempValue[MAX_BUFF_500] = {'\0'};

	//查找是否需要编写Insert
	char* pFunc = NULL;
	pFunc = strstr(obj_DB_Table.m_szDBFunc, "insert");
	if(NULL == pFunc)
	{
		return false;
	}

	//拼接参数
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(i != (int)obj_DB_Table.m_vec_DB_Column.size() - 1)
		{
			sprintf_safe(szTemp, MAX_BUFF_500, "%s%s, ", szTemp, obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
		}
		else
		{
			sprintf_safe(szTemp, MAX_BUFF_500, "%s%s", szTemp, obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
		}
	}

	//拼接结果
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(i != (int)obj_DB_Table.m_vec_DB_Column.size() - 1)
		{
			if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) > 0)
			{
				sprintf_safe(szTempValue, MAX_BUFF_500, "%sobj_%s.m_obj_%s, ", szTempValue, 
					obj_DB_Table.m_szClassName,
					obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
			}
			else
			{
				sprintf_safe(szTempValue, MAX_BUFF_500, "%sobj_%s, ", szTempValue, 
					obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
			}

		}
		else
		{
			if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) > 0)
			{
				sprintf_safe(szTempValue, MAX_BUFF_500, "%sobj_%s.m_obj_%s", szTempValue, 
					obj_DB_Table.m_szClassName,
					obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
			}
			else
			{
				sprintf_safe(szTempValue, MAX_BUFF_500, "%sobj_%s", szTempValue, 
					obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
			}
		}
	}

	sprintf_safe(pText, MAX_BUFF_1024, "\t//INSERT %s(%s) VALUES(%s);\n", obj_DB_Table.m_szTableName, 
		szTemp,
		szTempValue);

	return true;
}

bool Create_Update_SQL(_DB_Table& obj_DB_Table, char* pText)
{
	char szTemp[MAX_BUFF_500] = {'\0'};
	char szTempSelect[MAX_BUFF_500] = {'\0'};

	//查找是否需要编写Insert
	char* pFunc = NULL;
	pFunc = strstr(obj_DB_Table.m_szDBFunc, "update");
	if(NULL == pFunc)
	{
		return false;
	}

	//拼接参数
	int nPos = 0;
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(obj_DB_Table.m_vec_DB_Column[i].m_nIskey != 1)
		{
			if(nPos == 0)
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTemp, MAX_BUFF_500, "%s%s=obj_%s", szTemp, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTemp, MAX_BUFF_500, "%s%s=obj_%s.m_obj_%s", szTemp, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			else
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTemp, MAX_BUFF_500, "%s, %s=obj_%s", szTemp, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTemp, MAX_BUFF_500, "%s, %s=obj_%s.m_obj_%s", szTemp, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			nPos++;
		}
	}

	//拼接条件
	nPos = 0;
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(obj_DB_Table.m_vec_DB_Column[i].m_nIskey == 1)
		{
			if(nPos == 0)
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			else
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			nPos++;
		}
	}


	sprintf_safe(pText, MAX_BUFF_1024, "\t//UPDATE %s SET %s WHERE %s;\n", obj_DB_Table.m_szTableName, 
		szTemp,
		szTempSelect);

	return true;
}

bool Create_Delete_SQL(_DB_Table& obj_DB_Table, char* pText)
{
	char szTempSelect[MAX_BUFF_500] = {'\0'};

	//查找是否需要编写Insert
	char* pFunc = NULL;
	pFunc = strstr(obj_DB_Table.m_szDBFunc, "delete");
	if(NULL == pFunc)
	{
		return false;
	}

	//拼接条件
	int nPos = 0;
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(obj_DB_Table.m_vec_DB_Column[i].m_nIskey == 1)
		{
			if(nPos == 0)
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			else
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			nPos++;
		}
	}

	sprintf_safe(pText, MAX_BUFF_1024, "\t//DELETE FROM %s WHERE %s;\n", obj_DB_Table.m_szTableName, 
		szTempSelect);

	return true;
}

bool Create_Select_SQL(_DB_Table& obj_DB_Table, char* pText)
{
	char szTempSelect[MAX_BUFF_500] = {'\0'};

	//查找是否需要编写Insert
	char* pFunc = NULL;
	pFunc = strstr(obj_DB_Table.m_szDBFunc, "select");
	if(NULL == pFunc)
	{
		return false;
	}

	//拼接条件
	int nPos = 0;
	for(int i = 0; i < (int)obj_DB_Table.m_vec_DB_Column.size(); i++)
	{
		if(obj_DB_Table.m_vec_DB_Column[i].m_nIskey == 1)
		{
			if(nPos == 0)
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s%s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			else
			{
				if(strlen(obj_DB_Table.m_vec_DB_Column[i].m_szClassParam) == 0)
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName);
				}
				else
				{
					sprintf_safe(szTempSelect, MAX_BUFF_500, "%s AND %s=obj_%s.m_obj_%s", szTempSelect, 
						obj_DB_Table.m_vec_DB_Column[i].m_szDBName, 
						obj_DB_Table.m_szClassName, 
						obj_DB_Table.m_vec_DB_Column[i].m_szClassParam);
				}
			}
			nPos++;
		}
	}

	sprintf_safe(pText, MAX_BUFF_1024, "\t//SELECT * FROM %s WHERE %s;\n", obj_DB_Table.m_szTableName, 
		szTempSelect);

	return true;
}

void Create_DB_Environment(_DB_Proc& obj_DB_Proc)
{
	char szTempPath[MAX_BUFF_50]   = {'\0'};

	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_DB_Proc.m_szProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建LuaIncode目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DBWrapper", obj_DB_Proc.m_szProcName);
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

bool Create_DB_H(_DB_Proc& obj_DB_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.h", 
		obj_DB_Proc.m_szProcName);

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

	for(int i = 0; i < (int)obj_DB_Proc.m_vec_DB_Table.size(); i++)
	{
		//拼接参数
		char szParam[MAX_BUFF_100] = {'\0'};
		vec_DB_Column& obj_vec_DB_Column = obj_DB_Proc.m_vec_DB_Table[i].m_vec_DB_Column;
		int nParamIndex = 0;
		for(int j = 0; j < (int)obj_vec_DB_Column.size(); j++)
		{
			if(strlen(obj_vec_DB_Column[j].m_szClassParam) == 0)
			{
				//有外部参数，拼接之
				sprintf_safe(szParam, MAX_BUFF_100, "%s%s obj_%s, ", szParam, 
					obj_vec_DB_Column[j].m_szDBType, 
					obj_vec_DB_Column[j].m_szDBName);
				nParamIndex++;
			}
		}

		//增删改查
		char szTempSQL[MAX_BUFF_1024] = {'\0'};
		if(nParamIndex > 0)
		{
			if(Create_Insert_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Insert_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s);\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Update_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s);\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Delete_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Delete_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s);\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Select_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Select_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s);\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

		}
		else
		{
			if(Create_Insert_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Insert_%s(MysqlEncap* pMysqlEncap, %s obj_%s);\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Update_%s(MysqlEncap* pMysqlEncap, %s obj_%s);\n\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Delete_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Delete_%s(MysqlEncap* pMysqlEncap, %s obj_%s);\n\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Select_SQL(obj_DB_Proc.m_vec_DB_Table[i], szTempSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Select_%s(MysqlEncap* pMysqlEncap, %s obj_%s);\n\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

	}

	sprintf_safe(szTemp, 200, "#endif\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
	return true;
}

bool Create_DB_CPP(_DB_Proc& obj_DB_Proc)
{
	char szTemp[1024]         = {'\0'};
	char szPathFile[200]      = {'\0'};
	char szSQL[MAX_BUFF_1024] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.cpp", 
		obj_DB_Proc.m_szProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "#include \"DB_Op.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_DB_Proc.m_vec_DB_Table.size(); i++)
	{
		//拼接参数
		char szParam[MAX_BUFF_100] = {'\0'};
		vec_DB_Column& obj_vec_DB_Column = obj_DB_Proc.m_vec_DB_Table[i].m_vec_DB_Column;
		int nParamIndex = 0;
		for(int j = 0; j < (int)obj_vec_DB_Column.size(); j++)
		{
			if(strlen(obj_vec_DB_Column[j].m_szClassParam) == 0)
			{
				//有外部参数，拼接之
				sprintf_safe(szParam, MAX_BUFF_100, "%s%s obj_%s, ", szParam, 
					obj_vec_DB_Column[j].m_szDBType, 
					obj_vec_DB_Column[j].m_szDBName);
				nParamIndex++;
			}
		}

		if(nParamIndex > 0)
		{
			//增删改查
			if(Create_Insert_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Insert_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s)\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Update_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s)\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Delete_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Delete_%s(MysqlEncap* pMysqlEncap, %s%s obj_%s)\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Select_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Select_%s(MysqlEncap* pMysqlEncap, %s%s& obj_%s)\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					szParam, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		else
		{
			//增删改查

			if(Create_Insert_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Insert_%s(MysqlEncap* pMysqlEncap, %s obj_%s)\n", 
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Update_%s(MysqlEncap* pMysqlEncap, %s obj_%s)\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL);
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Delete_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Delete_%s(MysqlEncap* pMysqlEncap, %s obj_%s)\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL);
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			if(Create_Select_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL) == true)
			{
				sprintf_safe(szTemp, 200, "bool Select_%s(MysqlEncap* pMysqlEncap, %s& obj_%s)\n",
					obj_DB_Proc.m_vec_DB_Table[i].m_szTableName, 
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName,
					obj_DB_Proc.m_vec_DB_Table[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//填充函数内容
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//拼接SQL语句
				Create_Update_SQL(obj_DB_Proc.m_vec_DB_Table[i], szSQL);
				fwrite(szSQL, strlen(szSQL), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

	}

	fclose(pFile);
	return true;
}

void Create_DB_Proc(_DB_Proc& obj_DB_Proc)
{
	Create_DB_Environment(obj_DB_Proc);

	Create_DB_H(obj_DB_Proc);

	Create_DB_CPP(obj_DB_Proc);
}
