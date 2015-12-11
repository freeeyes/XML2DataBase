#ifndef _POOLTEST_H
#define _POOLTEST_H

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
#include <iostream>
#include <fstream>
using namespace std;

void Create_Pool_Test_Environment(_PoolTest_Group& obj_PoolTest_Group);

bool Create_Class_Pool_Test_Cpp(_PoolTest_Group& obj_PoolTest_Group);

bool CreatePool_Test(_PoolTest_Group& obj_PoolTest_Group);

#endif
