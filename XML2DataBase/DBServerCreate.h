#ifndef _DBSERVER_CREATE
#define _DBSERVER_CREATE

#include "Common.h"
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include<iostream>
#include<fstream>
using namespace std;

//对象池相关信息
struct _Pool_Info
{
	char m_sz_Key_Name[MAX_BUFF_50];
	char m_sz_Key_Type[MAX_BUFF_50];
	char m_sz_Class_Name[MAX_BUFF_50];

	_Pool_Info()
	{
		m_sz_Key_Name[0]   = '\0';
		m_sz_Key_Type[0]   = '\0';
		m_sz_Class_Name[0] = '\0';
	}
};
typedef vector<_Pool_Info> Vec_Save_Pool_Info;

void Create_DBServer_Environment(_XML_Proc& obj_XML_Proc);

bool Create_DB_Server_Main_CPP(_DB_Server_Info& obj_DB_Server_Info, _XML_Proc& obj_XML_Proc);

bool Create_DB_Server_Pool_H(_XML_Proc& obj_XML_Proc);

bool Create_DB_Server_Pool_CPP(_XML_Proc& obj_XML_Proc);

bool Create_DB_Server_Ini_File(_DB_Server_Info& obj_DB_Server_Info);

void Create_DB_Server(_DB_Server_Info& obj_DB_Server_Info, _XML_Proc& obj_XML_Proc);

#endif
