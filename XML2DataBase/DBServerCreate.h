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

void Create_DBServer_Environment(_XML_Proc& obj_XML_Proc);

bool Create_DB_Server_Class_CPP(_XML_Proc& obj_XML_Proc);

#endif
