#ifndef _DB_OPERATION_CREATE
#define _DB_OPERATION_CREATE

#include "Common.h"
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#endif
using namespace std;

bool Create_Insert_SQL(_DB_Table& obj_DB_Table, char* pText);

bool Create_Update_SQL(_DB_Table& obj_DB_Table, char* pText);

bool Create_Delete_SQL(_DB_Table& obj_DB_Table, char* pText);

bool Create_Select_SQL(_DB_Table& obj_DB_Table, char* pText);

void Create_DB_Environment(_DB_Proc& obj_DB_Proc);

bool Create_DB_H(_DB_Proc& obj_DB_Proc);

bool Create_DB_CPP(_DB_Proc& obj_DB_Proc);

void Create_DB_Proc(_DB_Proc& obj_DB_Proc);

#endif
