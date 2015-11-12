#include "ClassCreate.h"

void Check_Include_File(_Class_Info obj_Class_Info, _Proc_Info obj_Proc_Info, vec_Include_Info& obj_vec_Include_Info)
{
	obj_vec_Include_Info.clear();

	//遍历所有的属性
	for(int i = 0; i < (int)obj_Class_Info.m_vecProperty.size(); i++)
	{
		for(int j = 0; j < (int)obj_Proc_Info.obj_vec_Class_Info.size(); j++)
		{
			if(strcmp(obj_Class_Info.m_vecProperty[i].m_szProperyType, obj_Proc_Info.obj_vec_Class_Info[j].m_szXMLName)  == 0)
			{
				//比较是否已经存在
				bool blIsFind = false;
				for(int k = 0; (int)obj_vec_Include_Info.size(); k++)
				{
					if(strcmp(obj_Class_Info.m_vecProperty[i].m_szProperyType, obj_vec_Include_Info[k].m_szInclude) == 0)
					{
						blIsFind = true;
						break;
					}
				}

				if(false == blIsFind)
				{
					_Include_Info obj_Include_Info;
					sprintf_safe(obj_Include_Info.m_szInclude, MAX_BUFF_100, "%s", obj_Proc_Info.obj_vec_Class_Info[j].m_szXMLName);
					obj_vec_Include_Info.push_back(obj_Include_Info);
				}

				break;
			}
		}
	}
}

void Create_Environment(_Proc_Info& obj_Proc_Info)
{
	char szTempPath[MAX_BUFF_50]   = {'\0'};

	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_Proc_Info.m_szProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建LuaIncode目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DataWrapper", obj_Proc_Info.m_szProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif
}

void Create_Define_H(_Proc_Define_Info obj_Proc_Define_Info)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	//首先生成H文件。
	sprintf_safe(szPathFile, 200, "%s/DataWrapper/Common_Define.h", 
		obj_Proc_Define_Info.m_szProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return;
	}

	//编写文件说明
	sprintf_safe(szTemp, 200, "#ifndef _COMMON_DEFINE_H\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define _COMMON_DEFINE_H\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#include <stdio.h>\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_Proc_Define_Info.obj_vec_Define_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#define %s %s //%s\n", 
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szTagType,
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szSrcType,
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "#endif\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
}

bool Create_Class_H(_Proc_Info& obj_Proc_Info)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	for(int i = 0; i < (int)obj_Proc_Info.obj_vec_Class_Info.size(); i++)
	{
		sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s.h", 
			obj_Proc_Info.m_szProcName,
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);

		FILE* pFile = fopen(szPathFile, "wb");
		if(NULL == pFile)
		{
			return false;
		}

		//将类名转换成大写
		char szHText[MAX_BUFF_50] = {'\0'};
		To_Upper_String(obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName, szHText);

		sprintf_safe(szTemp, 200, "#ifndef _%s_H\n", szHText);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "#define _%s_H\n\n", szHText);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "#include \"Common_Define.h\"\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//检查是否需要包含别的类对象的头文件
		vec_Include_Info obj_vec_Include_Info;
		Check_Include_File((_Class_Info)obj_Proc_Info.obj_vec_Class_Info[i], obj_Proc_Info, obj_vec_Include_Info);

		for(int j = 0; j < (int)obj_vec_Include_Info.size(); j++)
		{
			sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_vec_Include_Info[j].m_szInclude);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//声明类对象
		sprintf_safe(szTemp, 200, "//%s\n", obj_Proc_Info.obj_vec_Class_Info[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "class %s\n", obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "public:\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		//构造函数
		sprintf_safe(szTemp, 200, "\tC%s();\n", obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t~%s();\n", obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//构造get和Set函数
		for(int j = 0; j < (int)obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty.size(); j++)
		{
			if(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength == 0)
			{
				sprintf_safe(szTemp, 200, "\tvoid set_%s(%s obj_%s);\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t%s get_%s();\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				if(strcmp(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType, "char") == 0)
				{
					//如果是字符串，特殊处理
					sprintf_safe(szTemp, 200, "\tvoid set_%s(char* obj_%s);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tchar* get_%s();\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tvoid set_%s(int nIndex, %s obj_%s);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tC%s get_%s(int nIndex);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//编写重载等于
		sprintf_safe(szTemp, 200, "\tC%s& operator = (const %s& ar)\n", 
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty.size(); j++)
		{
			if(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength == 0)
			{
				sprintf_safe(szTemp, 200, "\t\tthis->m_obj_%s = ar.m_obj_%s;\n",
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength > 0 && strcmp(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\t\tsprintf(this->m_obj_%s, \"%%s\", ar.m_obj_%s);\n",
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t\tfor(int i = 0; i < %d; i++)", obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t{");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tthis->m_obj_%s[i] = ar.m_obj_%s[i];\n",
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t}");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

		}
		sprintf_safe(szTemp, 200, "\t};\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "private:\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty.size(); j++)
		{
			if(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength > 0)
			{
				sprintf_safe(szTemp, 200, "\t%s m_obj_%s[%d]; //%s\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szDesc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t%s m_obj_%s; //%s\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szDesc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, "};\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//判断是否有Pool需要声明


		sprintf_safe(szTemp, 200, "#endif\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		fclose(pFile);
	}

	return true;
}

bool Create_Class_CPP(_Proc_Info& obj_Proc_Info)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	for(int i = 0; i < (int)obj_Proc_Info.obj_vec_Class_Info.size(); i++)
	{
		sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s.cpp", 
			obj_Proc_Info.m_szProcName,
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);

		FILE* pFile = fopen(szPathFile, "wb");
		if(NULL == pFile)
		{
			return false;
		}

		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "%s::%s()\n", 
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName, 
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "%s::~%s()\n", 
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName, 
			obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//构造get和Set函数
		for(int j = 0; j < (int)obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty.size(); j++)
		{
			if(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength == 0)
			{
				sprintf_safe(szTemp, 200, "void %s::set_%s(%s obj_%s)\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_obj_%s = obj_%s;\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "%s %s::get_%s()\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
					obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn m_obj_%s;\n", 
					obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				if(strcmp(obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType, "char") == 0)
				{
					//如果是字符串，特殊处理
					sprintf_safe(szTemp, 200, "void %s::set_%s(char* obj_%s)\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tsprintf(m_obj_%s, \"%%s\", obj_%s);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "char* %s::get_%s();\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\treturn m_obj_%s;\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "void %s::set_%s(int nIndex, %s obj_%s);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\treturn;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tm_obj_%s[nIndex] = obj_%s;\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "%s* %s::get_%s(int nIndex);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szProperyType,
						obj_Proc_Info.obj_vec_Class_Info[i].m_szXMLName,
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d);\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_nLength);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\treturn &m_obj_%s[nIndex];\n", 
						obj_Proc_Info.obj_vec_Class_Info[i].m_vecProperty[j].m_szPropertyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				}
			}
		}

		fclose(pFile);
	}

	return true;
}

void Create_Proc(_Proc_Define_Info obj_Proc_Define_Info, _Proc_Info& obj_Proc_Info)
{
	Create_Environment(obj_Proc_Info);

	Create_Define_H(obj_Proc_Define_Info);

	Create_Class_H(obj_Proc_Info);

	Create_Class_CPP(obj_Proc_Info);
}
