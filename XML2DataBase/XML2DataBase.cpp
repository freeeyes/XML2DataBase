// XML2DataBase.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "XmlOpeation.h"
#include "ClassCreate.h"
#include "DBOperationCreate.h"

int main(int argc, char* argv[])
{
	CXmlOpeation      obj_XmlOpeation;
	_Proc_Info        obj_Proc_Info;
	_Proc_Define_Info obj_Proc_Define_Info;
	_DB_Proc          obj_DB_Proc;

	obj_XmlOpeation.Parse_XML_Define_File("define.xml", obj_Proc_Define_Info);

	bool blState = obj_XmlOpeation.Parse_XML_Class_File("protocol.xml", obj_Proc_Info);
	if(false == blState)
	{
		printf("[Main]Get XML(protocol.xml) error.\n");
		printf("[Main]please any key to exit.\n");
		getchar();
	}

	obj_XmlOpeation.Parse_XML_DB_File("dbconfig.xml", obj_DB_Proc);

	Create_Proc(obj_Proc_Define_Info, obj_Proc_Info);

	Create_DB_Proc(obj_DB_Proc);

	return 0;
}

