#ifndef _CLASS_CREATE_H
#define _CLASS_CREATE_H

#include "Common.h"
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#endif
#include<iostream>
#include<fstream>
using namespace std;

void Check_Include_File(_Class_Info obj_Class_Info, _Proc_Info obj_Proc_Info, vec_Include_Info& obj_vec_Include_Info);

void Create_Environment(_Proc_Info& obj_Proc_Info);

void Create_Define_H(_Proc_Define_Info obj_Proc_Define_Info);

bool Create_Class_H(_Proc_Info& obj_Proc_Info);

bool Create_Class_CPP(_Proc_Info& obj_Proc_Info);

void Create_Proc(_Proc_Define_Info obj_Proc_Define_Info, _Proc_Info& obj_Proc_Info);

#endif
