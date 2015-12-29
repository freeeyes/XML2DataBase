// XML2DataBase.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "XmlOpeation.h"
#include "ClassCreate.h"
#include "DBOperationCreate.h"
#include "PoolTest.h"
#include "DBServerCreate.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

//遍历指定的目录，获得所有XML文件名
bool Read_Xml_Folder( string folderPath, vec_Xml_File_Name& obj_vec_Xml_File_Name)
{
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return false;
	}
	do
	{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//这个语句很重要
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				//不必支持子目录遍历
				//string newPath = folderPath + "\\" + FileInfo.name;
				//dfsFolder(newPath);
			}
		}
		else  
		{
			string filename = folderPath + "\\" + FileInfo.name;
			obj_vec_Xml_File_Name.push_back(filename);
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
		return false;
	}
	chdir(folderPath.c_str());
	while((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
				continue;
			//不需要支持子目录遍历
			//printf("%*s%s/\n",depth,"",entry->d_name);
			//dfsFolder(entry->d_name,depth+4);
		} 
		else 
		{
			string filename = folderPath + "/" + entry->d_name;
			obj_vec_Xml_File_Name.push_back(filename);
		}
	}
	chdir("..");
	closedir(dp);
#endif
	return true;
}

int main(int argc, char* argv[])
{
	CXmlOpeation      obj_XmlOpeation;

	_Proc_Define_Info obj_Proc_Define_Info;
	_XML_Proc         obj_XML_Proc;
	vec_Xml_File_Name obj_vec_Xml_File_Name;

	bool blRet = Read_Xml_Folder("XMLData", obj_vec_Xml_File_Name);
	if(false == blRet)
	{
		printf("[Main]Get XML(protocol.xml) error.\n");
		printf("[Main]please any key to exit.\n");
		getchar();
	}
	//printf("[Main]obj_vec_Xml_File_Name(%d).\n", obj_vec_Xml_File_Name.size());

	obj_XmlOpeation.Parse_XML_Define_File((char* )"define.xml", obj_Proc_Define_Info);
	sprintf_safe(obj_XML_Proc.m_sz_ProcName, MAX_BUFF_50, "%s", obj_Proc_Define_Info.m_szProcName);
	sprintf_safe(obj_XML_Proc.m_sz_DBType, MAX_BUFF_50, "%s", obj_Proc_Define_Info.m_szDBType);

	obj_XmlOpeation.Parse_XML_Table_Files(obj_vec_Xml_File_Name, obj_XML_Proc);

	//printf("[Main]vec_Table_Info(%d).\n", obj_XML_Proc.m_obj_vec_Table_Info.size());

	Create_Proc(obj_Proc_Define_Info, obj_XML_Proc);
	Create_DB_Proc(obj_XML_Proc);

	//生成测试池代码
	//_PoolTest_Group obj_PoolTest_Group;
	//obj_XmlOpeation.Parse_XML_Test_Pool((char* )"TestPool.xml", obj_PoolTest_Group);

	//CreatePool_Test(obj_XML_Proc, obj_PoolTest_Group);

	_DB_Server_Info obj_DB_Server_Info;
	obj_XmlOpeation.Parse_XML_DB_Server_File((char* )"DBServer.xml", obj_DB_Server_Info);
	Create_DBServer_Environment(obj_XML_Proc);
	Create_DB_Server_Main_CPP(obj_XML_Proc);

	//getchar();
	return 0;
}

