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
				string subfolderPath = folderPath + "/" + FileInfo.name;
				string newjosnPath = newfolderPath + "/" + FileInfo.name;

				//创建新的映射目录
				_mkdir(newjosnPath.c_str());

				Copy_Json_File(subfolderPath, newjosnPath);
			}
		}
		else  
		{
			string filename     = folderPath + "/" + FileInfo.name;
			string jsonfilename = newfolderPath + "/" + FileInfo.name;
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
					obj_Include_Info.m_n_Need_Logic_Class = obj_XML_Proc.m_obj_vec_Table_Info[j].m_n_Need_Logic_Class;
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

		//所有的类都有自己序列化的方法
		//你可以选择不用，但是会自己生成
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
			if(obj_vec_Include_Info[j].m_n_Need_Logic_Class == 1)
			{
				sprintf_safe(szTemp, 200, "#include \"%s_Logic.h\"\n", obj_vec_Include_Info[j].m_szInclude);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_vec_Include_Info[j].m_szInclude);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
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

		//增加获取key的接口
		if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) == 0)
				{
					if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
					{
						sprintf_safe(szTemp, 200, "\t%s getkey();\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\tchar* getkey();\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					break;
				}
			}
		}
		//构造get和Set函数
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				bool bfindLogic = false;
		
				for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
				{
					
					if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
						(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
					{
						sprintf_safe(szTemp, 200, "\tvoid set_%s(%s_Logic obj_%s);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t%s_Logic& get_%s();\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						bfindLogic = true;
						break;
					}
				}

				if (!bfindLogic)
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
			}
			else
			{
				//如果是数组
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

					bool bfindLogic = false;
		
					for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
					{
						
						if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
							(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
						{
							sprintf_safe(szTemp, 200, "\tvoid set_%s(int nIndex, %s_Logic obj_%s);\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t%s_Logic* get_%s(int nIndex);\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							//添加数组转化为Json的方法
							sprintf_safe(szTemp, 200, "\tstring get_%s();\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\tvoid set_%s(string strJson);\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							bfindLogic = true;
							break;
						}
					}

					if (!bfindLogic)
					{
						sprintf_safe(szTemp, 200, "\tvoid set_%s(int nIndex, %s obj_%s);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
						{
							sprintf_safe(szTemp, 200, "\t%s get_%s(int nIndex);\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							sprintf_safe(szTemp, 200, "\t%s* get_%s(int nIndex);\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}

						//添加数组转化为Json的方法
						sprintf_safe(szTemp, 200, "\tstring get_%s();\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\tvoid set_%s(string strJson);\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tvoid reset();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//看看是否要添加序列化的方法
		sprintf_safe(szTemp, 200, "\tstring serialization();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tvoid unserialization(string strSerial);\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

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
				sprintf_safe(szTemp, 200, "\t\tsnprintf(this->m_obj_%s,sizeof(this->m_obj_%s),\"%%s\", ar.m_obj_%s);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
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

		sprintf_safe(szTemp, 200, "protected:\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length > 0)
			{
				bool bfindLogic = false;
		
				for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
				{
					
					if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
						(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
					{
						sprintf_safe(szTemp, 200, "\t%s_Logic m_obj_%s[%d]; //%s\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						bfindLogic = true;
						break;
					}
				}

				if (!bfindLogic)
				{
					sprintf_safe(szTemp, 200, "\t%s m_obj_%s[%d]; //%s\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}				
			}
			else
			{

				bool bfindLogic = false;
				for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
				{
					
					if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
						(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
					{
						sprintf_safe(szTemp, 200, "\t%s_Logic m_obj_%s; //%s\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						bfindLogic = true;
						break;
					}
				}

				if (!bfindLogic)
				{
					sprintf_safe(szTemp, 200, "\t%s m_obj_%s; //%s\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Desc);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}	
			}
		}
		sprintf_safe(szTemp, 200, "};\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "#endif\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		fclose(pFile);

		//判断是否要生成逻辑类
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Need_Logic_Class == 1)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Logic.h", 
				obj_XML_Proc.m_sz_ProcName,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

			#ifdef WIN32
			if (!_access(szPathFile,0) ) 
			#else
			if (!access(szPathFile,0) ) 
			#endif
			{
				continue;
			}

			FILE* pFile = fopen(szPathFile, "wb");
			if(NULL == pFile)
			{
				return false;
			}

			//将类名转换成大写
			char szHText[MAX_BUFF_50] = {'\0'};
			To_Upper_String(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, szHText);

			sprintf_safe(szTemp, 200, "#ifndef _%s_LOGIC_H_\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#define _%s_LOGIC_H_\n\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#include \"%s.h\"\n",obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//声明类对象
			sprintf_safe(szTemp, 200, "//%s\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Desc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "class %s_Logic : public %s\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			//构造函数
			sprintf_safe(szTemp, 200, "\t%s_Logic();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t~%s_Logic();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "};\n\n");
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

		//检查是否需要包含别的类对象的头文件
		vec_Include_Info obj_vec_Include_Info;
		Check_Include_File((_Table_Info)obj_XML_Proc.m_obj_vec_Table_Info[i], obj_XML_Proc, obj_vec_Include_Info);

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
					sprintf_safe(szTemp, 200, "\tmemset(m_obj_%s,0,sizeof(m_obj_%s));\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
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
				sprintf_safe(szTemp, 200, "\t\treturn true;\n");
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
				sprintf_safe(szTemp, 200, "\t\treturn true;\n");
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
				sprintf_safe(szTemp, 200, "\t\t\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\treturn false;\n");
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

		//增加获取key的接口
		if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) == 0)
				{
					if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
					{
						sprintf_safe(szTemp, 200, "%s %s::getkey()\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
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
							sprintf_safe(szTemp, 200, "char* %s::getkey()\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							sprintf_safe(szTemp, 200, "{\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							sprintf_safe(szTemp, 200, "\treturn m_obj_%s;\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							sprintf_safe(szTemp, 200, "}\n\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
					}
					break;
				}
			}
		}	

		//构造get和Set函数
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				bool bfindLogic = false;
		
				for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
				{
					
					if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
						(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
					{
						sprintf_safe(szTemp, 200, "void %s::set_%s(%s_Logic obj_%s)\n", 
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

						sprintf_safe(szTemp, 200, "%s_Logic& %s::get_%s()\n", 
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
						bfindLogic = true;
						break;
					}
				}

				if (!bfindLogic)
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
					sprintf_safe(szTemp, 200, "\tsnprintf(m_obj_%s,sizeof(m_obj_%s),\"%%s\", obj_%s);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
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
					bool bfindLogic = false;
		
					for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
					{
						
						if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
							(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
						{
							sprintf_safe(szTemp, 200, "void %s::set_%s(int nIndex, %s_Logic obj_%s)\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							bfindLogic = true;
							break;
						}
					}
					if (!bfindLogic)
					{
						sprintf_safe(szTemp, 200, "void %s::set_%s(int nIndex, %s obj_%s)\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
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

					bfindLogic = false;
		
					for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
					{
						
						if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
							(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
						{
							sprintf_safe(szTemp, 200, "%s_Logic* %s::get_%s(int nIndex)\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							bfindLogic = true;
							break;
						}
					}
					if (!bfindLogic)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
						{
							sprintf_safe(szTemp, 200, "%s %s::get_%s(int nIndex)\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							sprintf_safe(szTemp, 200, "%s* %s::get_%s(int nIndex)\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
					}
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

					bfindLogic = false;
		
					for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
					{
						if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
							(strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
						{
							sprintf_safe(szTemp, 200, "\treturn &m_obj_%s[nIndex];\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							bfindLogic = true;
							break;
						}
					}
					if (!bfindLogic)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
						{
							sprintf_safe(szTemp, 200, "\treturn m_obj_%s[nIndex];\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							sprintf_safe(szTemp, 200, "\treturn &m_obj_%s[nIndex];\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}		
					}

					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					//把数组变成Json的string字符串
					sprintf_safe(szTemp, 200, "string %s::get_%s()\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					//test
					sprintf_safe(szTemp, 200, "\trapidjson::Document d;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\trapidjson::Document::AllocatorType& allocator = d.GetAllocator();\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\td.SetObject();\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					//test

					//数组数据,判断是否是当前已有的数据类型
					bool blFlag = false;
					for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
						{
							blFlag = true;
							break;
						}
					}
					if(true == blFlag)
					{
						//是自定义类
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
						//是基础类型
						sprintf_safe(szTemp, 200, "\trapidjson::Value array(rapidjson::kArrayType);\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						//obj.AddMember("age", 23, allocator)
						sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\t{\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t\tarray.PushBack(m_obj_%s[i], allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

						sprintf_safe(szTemp, 200, "\t}\n");
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\td.AddMember(\"array_%s\", array, allocator);\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
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

					//从Json还原成数组
					sprintf_safe(szTemp, 200, "void %s::set_%s(string strJson)\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					blFlag = false;
					for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
					{
						if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
						{
							blFlag = true;
							break;
						}
					}

					sprintf_safe(szTemp, 200, "\trapidjson::Document d;\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\td.Parse<rapidjson::kParseDefaultFlags>(strJson.c_str());\n\n");
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
							sprintf_safe(szTemp, 200, "\t\t\t\tm_obj_%s[i] = d[\"array_%s\"][i].GetInt();\n",
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

					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "}\n\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}

		sprintf_safe(szTemp, 200, "void %s::reset()\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
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
					sprintf_safe(szTemp, 200, "\tint i = 0;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\tfor(i = 0; i < %d; i++);\n", 
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
			else
			{
				if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
				{
					if(strcmp("int", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
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
						sprintf_safe(szTemp, 200, "\tm_obj_%s.reset();\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tint i = 0;\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					sprintf_safe(szTemp, 200, "\tfor(i = 0; i < %d; i++);\n", 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					if(strcmp("int", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
					{
						sprintf_safe(szTemp, 200, "\t\tm_obj_%s[i] = 0;\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else if(strcmp("char", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
					{
						sprintf_safe(szTemp, 200, "\t\tm_obj_%s[i][0] = '\0';\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
					else
					{
						sprintf_safe(szTemp, 200, "\t\tm_obj_%s[i].reset();\n", 
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}

					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}
		
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//必须生成对应序列化的方法，所有类对象统一
		sprintf_safe(szTemp, 200, "string %s::serialization()\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		//生成Json写入
		sprintf_safe(szTemp, 200, "\trapidjson::Document d;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\trapidjson::Document::AllocatorType& allocator = d.GetAllocator();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\td.SetObject();\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		//循环写入Json中的变量
		for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
		{
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0
				&& obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
			{
				//单一数据
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
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
					sprintf_safe(szTemp, 200, "\tvalue_%s.SetString(m_obj_%s.serialization().c_str(), m_obj_%s.serialization().size(), allocator);\n", 
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
				int k = 0;
				for( k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
				{
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
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

					sprintf_safe(szTemp, 200, "\t\trapidjson::Value object(rapidjson::kObjectType);\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

					for(int kLoop = 0; kLoop < (int)obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info.size(); kLoop++)
					{
						if (obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_n_Length > 0)
						{
							sprintf_safe(szTemp, 200, "\t\trapidjson::Value array%s(rapidjson::kArrayType);\n",
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\tfor(int j = 0; j < %d; j++)\n", 
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_n_Length);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							
							sprintf_safe(szTemp, 200, "\t\t{\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\tarray%s.PushBack(m_obj_%s[i].get_%s(j), allocator);\n",
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t}\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\tobject.AddMember(\"%s\",array%s,allocator);\n",
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							if (strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Class_Type, "char") == 0)
							{
								sprintf_safe(szTemp, 200, "\t\trapidjson::Value value_str%s;\n",
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

								sprintf_safe(szTemp, 200, "\t\tvalue_str%s.SetString(m_obj_%s[i].get_%s(),strlen(m_obj_%s[i].get_%s()),allocator);\n",
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

								sprintf_safe(szTemp, 200, "\t\tobject.AddMember(\"%s\",value_str%s,allocator);\n",
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							}
							else if (strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Class_Type, "int") == 0)
							{
								sprintf_safe(szTemp, 200, "\t\tobject.AddMember(\"%s\",m_obj_%s[i].get_%s(),allocator);\n",
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kLoop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							}
							else
							{
								printf("unkown data type!!!\n");
							}
						}
					}

					sprintf_safe(szTemp, 200, "\t\tarray.PushBack(object, allocator);\n");
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

					sprintf_safe(szTemp, 200, "\t\tarray.PushBack(m_obj_%s[i], allocator);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
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

		sprintf_safe(szTemp, 200, "void %s::unserialization(string strSerial)\n",
			obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		//生成Json读取
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
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
				{
					sprintf_safe(szTemp, 200, "\t\t\tm_obj_%s = d[\"%s\"].GetInt();\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\t\t\tm_obj_%s.unserialization(d[\"%s\"].GetString());\n",
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
				int k = 0;
				for(k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); k++)
				{
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_sz_Class_Name, 
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0)
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

					sprintf_safe(szTemp, 200, "\t\t\t\tif (d[\"array_%s\"][i].IsObject())\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					
					sprintf_safe(szTemp, 200, "\t\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					
					//此处要循环编译自定义对象属性 start
					for (int kloop = 0; kloop < (int)obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info.size(); ++kloop)
					{					
						if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Db_Type) > 0
							&& obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_n_Length == 0)
						{
							sprintf_safe(szTemp, 200, "\t\t\t\t\tif(d[\"array_%s\"][i].HasMember(\"%s\"))\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, 
								obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\t\t\t{\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Class_Type, "int") == 0)
							{
								sprintf_safe(szTemp, 200, "\t\t\t\t\t\tint obj_%s = d[\"array_%s\"][i][\"%s\"].GetInt();\n",
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

								sprintf_safe(szTemp, 200, "\t\t\t\t\t\tm_obj_%s[i].set_%s(obj_%s);\n",
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Column_Name,
									obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							}

							sprintf_safe(szTemp, 200, "\t\t\t\t\t}\n\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
						else
						{
							sprintf_safe(szTemp, 200, "\t\t\t\t\tif(d[\"array_%s\"][i].IsArray())\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\t\t\t{\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\t\t\t\tfor (int j = 0; j< d[\"array_%s\"][i].Capacity(); ++j)\n",
								obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\t\t\t\t{\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							
							if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[k].m_obj_vec_Column_Info[kloop].m_sz_Class_Type, "int") == 0)
							{
								sprintf_safe(szTemp, 200, "\t\t\t\t\t\t\trapidjson::Value& first = d[\"array_%s\"][i][j];\n",
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

								sprintf_safe(szTemp, 200, "\t\t\t\t\t\t\tstring strJson = (string)first.GetString();\n");
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

								sprintf_safe(szTemp, 200, "\t\t\t\t\t\t\tm_obj_%s[i].unserialization(strJson);\n",
									obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
								fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
							}

							sprintf_safe(szTemp, 200, "\t\t\t\t\t\t}\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

							sprintf_safe(szTemp, 200, "\t\t\t\t\t}\n\n");
							fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						}
					}
					//此处要循环编译自定义对象属性 end
					
					sprintf_safe(szTemp, 200, "\t\t\t\t}\n");
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
						sprintf_safe(szTemp, 200, "\t\t\t\tm_obj_%s[i] = d[\"array_%s\"][i].GetInt();\n",
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
					sprintf_safe(szTemp, 200, "\t\t\tsnprintf(m_obj_%s,sizeof(m_obj_%s),\"%%s\", d[\"%s\"].GetString());\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
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

		//判断是否要生成逻辑类
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Need_Logic_Class == 1)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Logic.cpp", 
				obj_XML_Proc.m_sz_ProcName,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

			#ifdef WIN32
			if (!_access(szPathFile,0) ) 
			#else
			if (!access(szPathFile,0) ) 
			#endif
			{
				continue;
			}

			FILE* pFile = fopen(szPathFile, "wb");
			if(NULL == pFile)
			{
				return false;
			}
			sprintf_safe(szTemp, 200, "#include \"%s_Logic.h\"\n",obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "%s_Logic::%s_Logic()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "%s_Logic::~%s_Logic()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
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
