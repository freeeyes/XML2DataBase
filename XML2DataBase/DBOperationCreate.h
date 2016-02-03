#ifndef _DB_OPERATION_CREATE
#define _DB_OPERATION_CREATE

#include "Common.h"
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#endif
using namespace std;

void Check_DB_Include_File(_Table_Info obj_Class_Info, _XML_Proc& obj_XML_Proc, vec_Include_Info& obj_vec_Include_Info);

void Create_DB_Environment(_XML_Proc& obj_XML_Proc);

bool Create_DB_H(_XML_Proc& obj_XML_Proc);

bool Create_DB_CPP(_XML_Proc& obj_XML_Proc);

void Create_DB_Proc(_XML_Proc& obj_XML_Proc);

#endif
