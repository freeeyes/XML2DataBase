#include "ClassCreate.h"

void Copy_Json_File(string folderPath, string newfolderPath)
{
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return;
	}
	do
	{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//这个语句很重要
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				string newfolderPath = folderPath + "\\" + FileInfo.name;
				string newjosnPath = newfolderPath + "\\" + FileInfo.name;

				//创建新的映射目录
				_mkdir(newjosnPath.c_str());

				Copy_Json_File(newfolderPath, newjosnPath);
			}
		}
		else  
		{
			string filename     = folderPath + "\\" + FileInfo.name;
			string jsonfilename = newfolderPath + "\\" + FileInfo.name;
			//开始拷贝文件
			Tranfile(filename.c_str(), jsonfilename.c_str());
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(folderPath.c_str())) == NULL) 
	{
		printf("cannot open directory: %s\n", folderPath.c_str());
		return;
	}
	//chdir(folderPath.c_str());
	while((entry = readdir(dp)) != NULL) 
	{
		string tmpName = folderPath + "/" + entry->d_name;
		lstat(tmpName.c_str(),&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			//printf("[Copy_Json_File]S_ISDIR=%s\n", entry->d_name);
			if(strcmp(".",entry->d_name) != 0 && strcmp("..",entry->d_name) != 0)
			{
				string subfolderPath = folderPath + "/" + entry->d_name;
				string newjosnPath = newfolderPath + "/" + entry->d_name;
				//printf("[Copy_Json_File]newfolderPath=%s\n", subfolderPath.c_str());
				//printf("[Copy_Json_File]newjosnPath=%s\n", newjosnPath.c_str());
				//创建新的映射目录
				mkdir(newjosnPath.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

				Copy_Json_File(subfolderPath, newjosnPath);
			}
		} 
		else 
		{
			//printf("[Copy_Json_File]file=%s\n", entry->d_name);
			string filename     = folderPath + "/" + entry->d_name;
			string jsonfilename = newfolderPath + "/" + entry->d_name;
			//开始拷贝文件
			Tranfile(filename.c_str(), jsonfilename.c_str());
		}
	}
	//chdir("..");
	closedir(dp);
#endif
}

void Copy_ShareMemory_File(string folderPath, string newfolderPath)
{
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return;
	}
	do
	{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//这个语句很重要
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				string newPath     = folderPath + "\\" + FileInfo.name;
				string newjosnPath = newfolderPath + "\\" + FileInfo.name;

				//创建新的映射目录
				_mkdir(newjosnPath.c_str());

				Copy_ShareMemory_File(newPath, newjosnPath);
			}
		}
		else  
		{
			string filename     = folderPath + "\\" + FileInfo.name;
			string jsonfilename = newfolderPath + "\\" + FileInfo.name;
			//开始拷贝文件
			Tranfile(filename.c_str(), jsonfilename.c_str());
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(folderPath.c_str())) == NULL) 
	{
		printf("cannot open directory: %s\n", folderPath.c_str());
		return;
	}

	while((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
			{
				continue;
			}

			string subfolderPath = folderPath + "/" + entry->d_name;
			string newjosnPath = newfolderPath + "/" + entry->d_name;
			//创建新的映射目录
			mkdir(newjosnPath.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

			Copy_ShareMemory_File(subfolderPath, newjosnPath);
		} 
		else 
		{
			string filename     = folderPath + "/" + entry->d_name;
			string jsonfilename = newfolderPath + "/" + entry->d_name;

			printf("filename: %s\n", filename.c_str());
			printf("jsonfilename: %s\n", jsonfilename.c_str());
			//开始拷贝文件
			Tranfile(filename.c_str(), jsonfilename.c_str());
		}
	}
	closedir(dp);
#endif
}

void Check_Include_File(_Table_Info obj_Class_Info, _XML_Proc& obj_XML_Proc, vec_Include_Info& obj_vec_Include_Info)
{
	obj_vec_Include_Info.clear();

	//遍历所有的属性
	for(int i = 0; i < (int)obj_Class_Info.m_obj_vec_Column_Info.size(); i++)
	{
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); j++)
		{
			if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Class_Type, obj_XML_Proc.m_obj_vec_Table_Info[j].m_sz_Class_Name)  == 0)
			{
				//比较是否已经存在
				bool blIsFind = false;
				for(int k = 0; (int)obj_vec_Include_Info.size(); k++)
				{
					if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Class_Type, obj_vec_Include_Info[k].m_szInclude) == 0)
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
	//创建工程文件夹
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建Json目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/rapidjson", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建DataWrapper目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DataWrapper", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建ShareMemory目录
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/ShareMemory", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//拷贝MD5文件
	char szTempFile[MAX_BUFF_100] = {'\0'};
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/DataWrapper/MD5.h", obj_XML_Proc.m_sz_ProcName);
	Tranfile("../MD5.h", szTempFile);
	sprintf_safe(szTempFile, MAX_BUFF_100, "%s/DataWrapper/MD5.cpp", obj_XML_Proc.m_sz_ProcName);
	Tranfile("../MD5.cpp", szTempFile);
}

void Create_Define_H(_Proc_Define_Info& obj_Proc_Define_Info)
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
	sprintf_safe(szTemp, 200, "#ifndef _COMMON_DEFINE_H_\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define _COMMON_DEFINE_H_\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#include <stdio.h>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#include <iostream>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "using namespace std;\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_Proc_Define_Info.obj_vec_Define_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#define %s %s //%s\n", 
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szTagType,
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szSrcType,
			obj_Proc_Define_Info.obj_vec_Define_Info[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//编写枚举代码
	for(int i = 0; i < (int)obj_Proc_Define_Info.obj_vec_Enum_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "enum %s\n",
			obj_Proc_Define_Info.obj_vec_Enum_Info[i].m_szEnum);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_Proc_Define_Info.obj_vec_Enum_Info[i].obj_vec_Enum_Name_Info.size(); j++)
		{
			if (j + 1 == obj_Proc_Define_Info.obj_vec_Enum_Info[i].obj_vec_Enum_Name_Info.size())
			{
				sprintf_safe(szTemp, 200, "\t%s = %d\n",
					obj_Proc_Define_Info.obj_vec_Enum_Info[i].obj_vec_Enum_Name_Info[j].m_szEnumName,
					j);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t%s = %d,\n",
					obj_Proc_Define_Info.obj_vec_Enum_Info[i].obj_vec_Enum_Name_Info[j].m_szEnumName,
					j);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, "};\n\n");
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
		sprintf_safe(szTemp, 200, "#include \"MD5.h\"\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//看看是否要添加序列化的方法
		if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_SerialType, "json") == 0)
		{
			sprintf_safe(szTemp, 200, "#include \"rapidjson/rapidjson.h\"\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#include \"rapidjson/document.h\"\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#include \"rapidjson/writer.h\"\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#include \"rapidjson/stringbuffer.h\"\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "using namespace rapidjson;\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
		{
			sprintf_safe(szTemp, 200, "#include \"ShareMemory.h\"\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

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
			sprintf_safe(szTemp, 200, "#include <vector>\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		sprintf_safe(szTemp, 200, "#include <string>\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "using namespace std;\n\n");
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
		sprintf_safe(szTemp, 200, "\t%s();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t~%s();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//初始化函数
		sprintf_safe(szTemp, 200, "\tvoid init();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//初始化数据比较函数
		sprintf_safe(szTemp, 200, "\tbool check_init();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//MD5函数
		sprintf_safe(szTemp, 200, "\tvoid get_md5(char* pMD5);\n");
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

					sprintf_safe(szTemp, 200, "\t%s* get_%s(int nIndex);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//看看是否要添加序列化的方法
		if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_SerialType, "json") == 0)
		{
			sprintf_safe(szTemp, 200, "\tstring serialization();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tvoid unserialization(string& strSerial);\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}

		//编写重载等于
		sprintf_safe(szTemp, 200, "\t%s& operator= (const %s& ar)\n", 
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
				sprintf_safe(szTemp, 200, "\t\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tthis->m_obj_%s[i] = ar.m_obj_%s[i];\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, "\t\treturn *this;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
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

		sprintf_safe(szTemp, 200, "#endif\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		fclose(pFile);
	}

	//生成对应的pool头文件
	for(int i =0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		//判断是否有Pool需要声明
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Pool.h", 
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

			sprintf_safe(szTemp, 200, "#ifndef _%s_POOL_H\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#define _%s_POOL_H\n\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//添加引用基类头文件
			sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

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

			//声明map和vector
			sprintf_safe(szTemp, 200, "typedef vector<%s*> vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "typedef map<%s, %s*> map_%s;\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//生成pool相应代码
			sprintf_safe(szTemp, 200, "//Pool Manager %s.\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "class %s_Pool\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t~%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//生成对应调用方法
			sprintf_safe(szTemp, 200, "\tvoid init();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//加载数据函数
			//sprintf_safe(szTemp, 200, "\tbool load_data();\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//保存数据函数
			//sprintf_safe(szTemp, 200, "\tbool save_data();\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tbool get_is_create();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tvoid set_is_create(bool bl_is_create);\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tsize_t get_st_poolsize();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* get_free_object(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* get_used_object(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tbool del_used_object(%s obj_%s);\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint get_used_count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint get_pool_count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//重置所有对象
			sprintf_safe(szTemp, 200, "\tvoid clear();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
			{
				sprintf_safe(szTemp, 200, "\tshm_id m_obj_shm_id;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			sprintf_safe(szTemp, 200, "\t%s* m_list_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tint m_list_Count;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tbool m_bl_Is_Create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tsize_t m_st_PoolSize;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tvec_%s m_vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s m_map_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//添加单件代码
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tstatic %s_Pool* obj_%s_Pool_Instance;\n\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tclass CClear_%s_Instance\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tpublic:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tCClear_%s_Instance() {};\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t~CClear_%s_Instance()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tdelete obj_%s_Pool_Instance;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t};\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tstatic %s_Pool* getInstance()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tif(obj_%s_Pool_Instance == NULL)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tstatic CClear_%s_Instance obj_%s_clr;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tobj_%s_Pool_Instance = new %s_Pool();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tobj_%s_Pool_Instance->init();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn obj_%s_Pool_Instance;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tmap_%s& get_used_map()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn m_map_%s;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "};\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "#endif\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			fclose(pFile);
		}
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

		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//sprintf_safe(szTemp, sizeof(szTemp), "#include \"DB_Op.h\"\n\n");
		//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "%s::%s()\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tinit();\n");
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
		sprintf_safe(szTemp, 200, "void %s::init()\n", 
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
		sprintf_safe(szTemp, 200, "}\n\n", 
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//初始化数据比较函数
		sprintf_safe(szTemp, 200, "bool %s::check_init()\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0
				&& strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value) > 0)
			{
				sprintf_safe(szTemp, 200, "\tif(m_obj_%s != %s)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn false;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0
				&& strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value) > 0)
			{
				sprintf_safe(szTemp, 200, "\tif(strcmp(m_obj_%s, \"\") != 0)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn false;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value) > 0)
			{
				sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tif(m_obj_%s[i] != %s)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Init_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\treturn false;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\treturn true;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//MD5函数
		sprintf_safe(szTemp, 200, "void %s::get_md5(char* pMD5)\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tCMD5 objMD5;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tobjMD5.ENCODE_DATA((char* )this, sizeof(%s), pMD5);\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
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

					sprintf_safe(szTemp, 200, "char* %s::get_%s()\n", 
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
					sprintf_safe(szTemp, 200, "void %s::set_%s(int nIndex, %s obj_%s)\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d)\n", 
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

					sprintf_safe(szTemp, 200, "%s* %s::get_%s(int nIndex)\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tif(nIndex < 0 && nIndex >= %d)\n", 
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
		}

		//看看是否要添加序列化的方法
		if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_SerialType, "json") == 0)
		{
			sprintf_safe(szTemp, 200, "string %s::serialization()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			//生成Json写入
			sprintf_safe(szTemp, 200, "\trapidjson::Document d;\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\trapidjson::Document::AllocatorType& allocator = d.GetAllocator();\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\td.SetObject();\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			//循环写入Json中的变量
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0
					&& obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
				{
					//单一数据
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "int") == 0)
					{
						sprintf_safe(szTemp, 200, "\td.AddMember(\"%s\", m_obj_%s, allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\trapidjson::Value value_%s;\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\tvalue_%s.SetString(m_obj_%s, strlen(m_obj_%s), allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\td.AddMember(\"%s\", value_%s, allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
				else if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0
					&& obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length > 0
					&& strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") != 0)
				{
					//数组数据,判断是否是当前已有的数据类型
					bool blFlag = false;
					for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
						{
							blFlag = true;
							break;
						}
					}

					if(true == blFlag)
					{
						//当前声明类对象，调用Json序列化方法
						sprintf_safe(szTemp, 200, "\trapidjson::Value array(rapidjson::kArrayType);\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						//obj.AddMember("age", 23, allocator)
						sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\trapidjson::Value value_str;\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\tvalue_str.SetString(m_obj_%s[i].serialization().c_str(),m_obj_%s[i].serialization().size(), allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\tarray.PushBack(value_str, allocator);\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t}\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\td.AddMember(\"array_%s\", array, allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						//基础类型
						sprintf_safe(szTemp, 200, "\trapidjson::Value array(rapidjson::kArrayType);\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						//obj.AddMember("age", 23, allocator)
						sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\trapidjson::Value value_str;\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\tvalue_str.SetString(m_obj_%s[i].serialization().c_str(),m_obj_%s[i].serialization().size(), allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\tarray.PushBack(value_str, allocator);\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						
						sprintf_safe(szTemp, 200, "\t}\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\td.AddMember(\"array_%s\", array, allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
				else
				{
					//单一数据
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "int") == 0)
					{
						sprintf_safe(szTemp, 200, "\td.AddMember(\"%s\", m_obj_%s, allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\trapidjson::Value value_%s;\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);	
						sprintf_safe(szTemp, 200, "\tvalue_%s.SetString(m_obj_%s, strlen(m_obj_%s), allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\td.AddMember(\"%s\", value_%s, allocator);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
			}
			sprintf_safe(szTemp, 200, "\tStringBuffer buffer;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\trapidjson::Writer<StringBuffer> writer(buffer);\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\td.Accept(writer);\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn string(buffer.GetString());\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "void %s::unserialization(string& strSerial)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			//生成Json读取
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\trapidjson::Document d;\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\td.Parse<rapidjson::kParseDefaultFlags>(strSerial.c_str());\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif (d.HasParseError())\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif (d.IsObject())\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//循环读取Json中的变量
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0
					&& obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
				{
					sprintf_safe(szTemp, 200, "\t\tif(d.HasMember(\"%s\"))\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "int") == 0)
					{
						sprintf_safe(szTemp, 200, "\t\t\tm_obj_%s = d[\"%s\"].GetInt();\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\t\t\tsprintf(m_obj_%s, \"%%s\", d[\"%s\"].GetString());\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}

					sprintf_safe(szTemp, 200, "\t\t}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0
					&& obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length > 0
					&& strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") != 0)
				{
					//数组数据,判断是否是当前已有的数据类型
					bool blFlag = false;
					for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
						{
							blFlag = true;
							break;
						}
					}

					if(true == blFlag)
					{
						//当前声明类对象，调用Json序列化方法
						sprintf_safe(szTemp, 200, "\t\tif(d.HasMember(\"array_%s\"))\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t\tfor(int i = 0; i < (int)d[\"array_%s\"].Size(); i++)\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\t\t\tstring strJson = (string)d[\"array_%s\"][i].GetString();\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t\t\tm_obj_%s[i].unserialization(strJson);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						
						sprintf_safe(szTemp, 200, "\t\t\t}\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\t}\n\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						//基础类型
						sprintf_safe(szTemp, 200, "\t\tif(d.HasMember(\"array_%s\"))\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\t\tfor(int i = 0; i < (int)d[\"array_%s\"].Size(); i++)\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "int") == 0)
						{
							sprintf_safe(szTemp, 200, "\t\t\t\tm_obj_%s = d[\"array_%s\"][i].GetInt()\");\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							sprintf_safe(szTemp, 200, "\t\t\t\tstring strJson = (string)d[\"array_%s\"][i].GetString();\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							
							sprintf_safe(szTemp, 200, "\t\t\t\tm_obj_%s[i].unserialization(strJson);\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						sprintf_safe(szTemp, 200, "\t\t\t}\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t\t}\n\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
				else
				{
					sprintf_safe(szTemp, 200, "\t\tif(d.HasMember(\"%s\"))\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "int") == 0)
					{
						sprintf_safe(szTemp, 200, "\t\t\tm_obj_%s = d[\"%s\"].GetInt();\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\t\t\tsprintf(m_obj_%s, \"%%s\", d[\"%s\"].GetString());\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}

					sprintf_safe(szTemp, 200, "\t\t}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}

			}

			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			fclose(pFile);
		}
	}


	//生成对应Pool代码
	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		//判断是否有Pool需要声明
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Pool.cpp", 
				obj_XML_Proc.m_sz_ProcName,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

			FILE* pFile = fopen(szPathFile, "wb");
			if(NULL == pFile)
			{
				return false;
			}

			sprintf_safe(szTemp, 200, "#include \"%s_Pool.h\"\n\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//sprintf_safe(szTemp, sizeof(szTemp), "#include \"DB_Op.h\"\n\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0)
			{
				//添加单件初始化
				sprintf_safe(szTemp, 200, "%s_Pool *%s_Pool::obj_%s_Pool_Instance = NULL;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

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
			sprintf_safe(szTemp, 200, "\tm_list_%s = NULL;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_list_Count = 0;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_bl_Is_Create = true;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_st_PoolSize = 0;\n");
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
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) == 0)
			{
				sprintf_safe(szTemp, 200, "\tif(NULL != m_list_%s)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tdelete[] m_list_%s;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_list_%s = NULL;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//释放共享内存连接
				sprintf_safe(szTemp, 200, "\tif(NULL != m_list_%s && m_obj_shm_id > 0)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tClose_Share_Memory_API((char* )m_list_%s, m_obj_shm_id, get_st_poolsize());\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//初始化
			sprintf_safe(szTemp, 200, "void %s_Pool::init()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//这里判断是否有共享内存标签，如果有填充共享内存代码
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
			{
				sprintf_safe(szTemp, 200, "\tm_st_PoolSize = sizeof(%s) * %d;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_list_%s = (%s* )Open_Share_Memory_API(%s, m_st_PoolSize, m_obj_shm_id, m_bl_Is_Create);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tm_list_%s = new %s[%d];\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			sprintf_safe(szTemp, 200, "\tif(NULL == m_list_%s)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_list_Count = %d;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			if (obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_IsDependFunc == 1)
			{
				//test
				sprintf_safe(szTemp, 200, "\tif(m_bl_Is_Create)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tload_data_from_%s(m_list_%s, m_list_Count);\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//test

				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
				{
					//开始根据共享内存还原数据结构map和vector
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tif(m_bl_Is_Create == false)\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tif(true == m_list_%s[i].check_init())\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					//还原内存场景
					sprintf_safe(szTemp, 200, "\t\t\t\tm_map_%s.insert(map_%s::value_type(m_list_%s[i].get_%s(), &m_list_%s[i]));\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
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
				}
			}
			else
			{
				//test
				sprintf_safe(szTemp, 200, "\tif(m_bl_Is_Create)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\tmap_%s mapObject = %s_Pool::get_used_map();\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			    sprintf_safe(szTemp, 200, "\t\tint iLoop = 0;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\tfor(map_%s::iterator b = mapObject.begin(); b != mapObject.end(); b++,iLoop++)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t\tif(iLoop < m_list_Count)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				for(int iloop = 0; iloop < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); iloop++)
				{
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass, obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_Class_Name) == 0)
					{
						sprintf_safe(szTemp, 200, "\t\t\t\tm_list_%s[iLoop].set_%s(((%s*)b->second)->get_%s());\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
							obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_Class_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_key);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						break;
					}
				}

				sprintf_safe(szTemp, 200, "\t\t\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				
				sprintf_safe(szTemp, 200, "\t\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//test

				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
				{
					//开始根据共享内存还原数据结构map和vector
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tif(m_bl_Is_Create == false)\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tif(true == m_list_%s[i].check_init())\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					//还原内存场景
					sprintf_safe(szTemp, 200, "\t\t\t\tm_map_%s.insert(map_%s::value_type(m_list_%s[i].get_%s(), &m_list_%s[i]));\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
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
				}
				
			}

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			/*
			sprintf_safe(szTemp, 200, "bool %s_Pool::load_data()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			
			sprintf_safe(szTemp, 200, "\treturn load_data_from_%s(m_list_%s, m_list_Count);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "bool %s_Pool::save_data()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\treturn save_data_to_%s(m_list_%s, m_list_Count);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			*/


			sprintf_safe(szTemp, 200, "bool %s_Pool::get_is_create()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn m_bl_Is_Create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "void %s_Pool::set_is_create(bool bl_is_create)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_bl_Is_Create = bl_is_create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "size_t %s_Pool::get_st_poolsize()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn m_st_PoolSize;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到空余的对象
			sprintf_safe(szTemp, 200, "%s* %s_Pool::get_free_object(%s obj_%s)\n", 
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
			sprintf_safe(szTemp, 200, "\tm_map_%s.insert(map_%s::value_type(obj_%s, p%s));\n",
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
			sprintf_safe(szTemp, 200, "%s* %s_Pool::get_used_object(%s obj_%s)\n", 
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
			sprintf_safe(szTemp, 200, "bool %s_Pool::del_used_object(%s obj_%s)\n", 
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
			sprintf_safe(szTemp, 200, "\t\tp%s->init();\n",
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
			sprintf_safe(szTemp, 200, "int %s_Pool::get_used_count()\n", 
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
			sprintf_safe(szTemp, 200, "int %s_Pool::get_pool_count()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn (int)m_vec_%s.size();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//全部重置对象函数
			sprintf_safe(szTemp, 200, "void %s_Pool::clear()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_vec_%s.clear();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_map_%s.clear();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmemset(m_list_%s, 0, m_st_PoolSize);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
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

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);	

			fclose(pFile);
		}
	}

	return true;
}

void Create_Proc(_Proc_Define_Info& obj_Proc_Define_Info, _XML_Proc& obj_XML_Proc)
{
	Create_Environment(obj_XML_Proc);

	char szSrcPath[MAX_BUFF_100] = {'\0'};
	char szTagpath[MAX_BUFF_100] = {'\0'};
	sprintf_safe(szSrcPath, MAX_BUFF_100, "../rapidjson");
	sprintf_safe(szTagpath, MAX_BUFF_100, "./%s/rapidjson", obj_XML_Proc.m_sz_ProcName);
	Copy_Json_File(szSrcPath, szTagpath);

	sprintf_safe(szSrcPath, MAX_BUFF_100, "../ShareMemory");
	sprintf_safe(szTagpath, MAX_BUFF_100, "./%s/ShareMemory", obj_XML_Proc.m_sz_ProcName);
	Copy_ShareMemory_File(szSrcPath, szTagpath);

	Create_Define_H(obj_Proc_Define_Info);

	Create_Class_H(obj_XML_Proc);

	Create_Class_CPP(obj_XML_Proc);
}
