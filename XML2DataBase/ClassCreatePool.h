#ifndef _CLASS_CREATE_POOL_H
#define _CLASS_CREATE_POOL_H

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

bool Create_Class_Pool_H(_XML_Proc& obj_XML_Proc);

bool Create_Class_Pool_CPP(_XML_Proc& obj_XML_Proc);

void Create_Pool_Proc(_XML_Proc& obj_XML_Proc);

#endif
