#include "ClassCreate.h"

void Check_Include_File(_Table_Info obj_Class_Info, _XML_Proc obj_XML_Proc, vec_Include_Info& obj_vec_Include_Info)
{
	obj_vec_Include_Info.clear();

	//遍历所有的属性
	for(int i = 0; i < (int)obj_Class_Info.m_obj_vec_Column_Info.size(); i++)
	{
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); j++)
		{
			if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[j].m_sz_Class_Name)  == 0)
			{
				//比较是否已经存在
				bool blIsFind = false;
				for(int k = 0; (int)obj_vec_Include_Info.size(); k++)
				{
					if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Column_Name, obj_vec_Include_Info[k].m_szInclude) == 0)
					{
						blIsFind = true;
						break;
					}
				}

				if(false == blIsFind)
				{
					_Include_Info obj_Include_Info;
					sprintf_safe(obj_Include_Info.m_szInclude, MAX_BUFF_100, "%s", obj_XML_Proc.m_obj_vec_Table_Info[j].m_sz_Class_Name);
					obj_vec_Include_Info.push_back(obj_Include_Info);
				}

				break;
			}
		}
	}
}

void Create_Environment(_XML_Proc& obj_XML_Proc)
{
	char szTempPath[MAX_BUFF_50]   = {'\0'};

	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建LuaIncode目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DataWrapper", obj_XML_Proc.m_sz_ProcName);
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

bool Create_Class_H(_XML_Proc& obj_XML_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s.h", 
			obj_XML_Proc.m_sz_ProcName,
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

		FILE* pFile = fopen(szPathFile, "wb");
		if(NULL == pFile)
		{
			return false;
		}

		//将类名转换成大写
		char szHText[MAX_BUFF_50] = {'\0'};
		To_Upper_String(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, szHText);

		sprintf_safe(szTemp, 200, "#ifndef _%s_H\n", szHText);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "#define _%s_H\n\n", szHText);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "#include \"Common_Define.h\"\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//检查是否需要包含别的类对象的头文件
		vec_Include_Info obj_vec_Include_Info;
		Check_Include_File((_Table_Info)obj_XML_Proc.m_obj_vec_Table_Info[i], obj_XML_Proc, obj_vec_Include_Info);

		for(int j = 0; j < (int)obj_vec_Include_Info.size(); j++)
		{
			sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_vec_Include_Info[j].m_szInclude);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			sprintf_safe(szTemp, 200, "#include <map>\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "using namespace std;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//声明类对象
		sprintf_safe(szTemp, 200, "//%s\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Desc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "class %s\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "public:\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		//构造函数
		sprintf_safe(szTemp, 200, "\tC%s();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t~%s();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//初始化函数
		sprintf_safe(szTemp, 200, "\tvoid Init();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//构造get和Set函数
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				sprintf_safe(szTemp, 200, "\tvoid set_%s(%s obj_%s);\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t%s get_%s();\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
				{
					//如果是字符串，特殊处理
					sprintf_safe(szTemp, 200, "\tvoid set_%s(char* obj_%s);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tchar* get_%s();\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tvoid set_%s(int nIndex, %s obj_%s);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tC%s get_%s(int nIndex);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//编写重载等于
		sprintf_safe(szTemp, 200, "\tC%s& operator = (const %s& ar)\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				sprintf_safe(szTemp, 200, "\t\tthis->m_obj_%s = ar.m_obj_%s;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length > 0 && strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\t\tsprintf(this->m_obj_%s, \"%%s\", ar.m_obj_%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t\tfor(int i = 0; i < %d; i++)", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t{");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tthis->m_obj_%s[i] = ar.m_obj_%s[i];\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t}");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

		}
		sprintf_safe(szTemp, 200, "\t};\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "private:\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length > 0)
			{
				sprintf_safe(szTemp, 200, "\t%s m_obj_%s[%d]; //%s\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t%s m_obj_%s; //%s\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, "};\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//判断是否有Pool需要声明
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			//首先得到key的类型
			char szKeyType[MAX_BUFF_50] = {'\0'};
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
				{
					sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					break;
				}
			}

			//如果得不到Key类型，则不生成Pool代码
			if(strlen(szKeyType) == 0)
			{
				continue;
			}

			//生成pool相应代码
			sprintf_safe(szTemp, 200, "//Pool Manager %s.\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "class %s_Pool\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t~%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//生成对应调用方法
			sprintf_safe(szTemp, 200, "\tvoid Init();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* Get_Free_%s(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* Get_Used_%s(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tbool Del_Used_%s(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint Get_Used_Count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint Get_Pool_Count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t%s m_list_%s[%d];\n\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//声明map和vector
			sprintf_safe(szTemp, 200, "\ttypedef vector<%s*> vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\ttypedef map<%s, %s*> map_%s;\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tvec_%s m_vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s m_map_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "};\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		sprintf_safe(szTemp, 200, "#endif\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		fclose(pFile);
	}

	return true;
}

bool Create_Class_CPP(_XML_Proc& obj_XML_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s.cpp", 
			obj_XML_Proc.m_sz_ProcName,
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

		FILE* pFile = fopen(szPathFile, "wb");
		if(NULL == pFile)
		{
			return false;
		}

		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "%s::%s()\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tInit();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "%s::~%s()\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//添加初始化函数
		sprintf_safe(szTemp, 200, "void %s::Init()\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value) > 0)
			{
				if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
				{
					sprintf_safe(szTemp, 200, "\tm_obj_%s = %s;\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(strcmp("char", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
				{
					sprintf_safe(szTemp, 200, "\tm_obj_%s[0] = %s;\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %s; i++);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tm_obj_%s[i] = %s;\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}
		sprintf_safe(szTemp, 200, "}\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//构造get和Set函数
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				sprintf_safe(szTemp, 200, "void %s::set_%s(%s obj_%s)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_obj_%s = obj_%s;\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "%s %s::get_%s()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn m_obj_%s;\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
				{
					//如果是字符串，特殊处理
					sprintf_safe(szTemp, 200, "void %s::set_%s(char* obj_%s)\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tsprintf(m_obj_%s, \"%%s\", obj_%s);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "char* %s::get_%s();\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\treturn m_obj_%s;\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "void %s::set_%s(int nIndex, %s obj_%s);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\treturn;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tm_obj_%s[nIndex] = obj_%s;\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "%s* %s::get_%s(int nIndex);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\treturn &m_obj_%s[nIndex];\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				}
			}

			//判断是否有Pool需要声明
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
			{
				//首先得到key的类型
				char szKeyType[MAX_BUFF_50] = {'\0'};
				for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
				{
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
					{
						sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
						break;
					}
				}

				//如果得不到Key类型，则不生成Pool代码
				if(strlen(szKeyType) == 0)
				{
					continue;
				}

				//填充函数
				//构造
				sprintf_safe(szTemp, 200, "%s_Pool::%s_Pool()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//析构
				sprintf_safe(szTemp, 200, "%s_Pool::~%s_Pool()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//初始化
				sprintf_safe(szTemp, 200, "void %s_Pool::Init()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//得到空余的对象
				sprintf_safe(szTemp, 200, "%s* %s_Pool::Get_Free_%s(%s obj_%s)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					szKeyType,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(m_vec_%s.size() <= 0)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t%s* p%s = NULL;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tvec_%s::iterator b = m_vec_%s.begin();\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tp%s = (%s* )(*b);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn (%s* )f->second;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_map_%s.insert(map_%s::value_type(obj_%s, p%s)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_vec_%s.erase(b);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn p%s;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//得到正在使用的对象函数
				sprintf_safe(szTemp, 200, "%s* %s_Pool::Get_Used_%s(%s obj_%s)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					szKeyType,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn (%s* )f->second;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\telse\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//归还正在使用的对象函数
				sprintf_safe(szTemp, 200, "bool %s_Pool::Del_Used_%s(%s obj_%s)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					szKeyType,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t%s* p%s = (%s* )f->second;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_map_%s.erase(f);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(p%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\telse\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn false;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//得到正在使用的对象个数函数
				sprintf_safe(szTemp, 200, "bool %s_Pool::Get_Used_Count()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn (int)m_map_%s.size();\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				//得到池里面总共的对象个数函数
				sprintf_safe(szTemp, 200, "bool %s_Pool::Get_Pool_Count()\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn (int)m_vec_%s.size();\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);	

			}
		}

		fclose(pFile);
	}

	return true;
}

void Create_Proc(_Proc_Define_Info obj_Proc_Define_Info, _XML_Proc obj_XML_Proc)
{
	Create_Environment(obj_XML_Proc);

	Create_Define_H(obj_Proc_Define_Info);

	Create_Class_H(obj_XML_Proc);

	Create_Class_CPP(obj_XML_Proc);
}
