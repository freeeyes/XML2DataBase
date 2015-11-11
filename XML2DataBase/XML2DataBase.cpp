// XML2DataBase.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "XmlOpeation.h"
#include "ClassCreate.h"

int main(int argc, char* argv[])
{
	CXmlOpeation      obj_XmlOpeation;
	_Proc_Info        obj_Proc_Info;
	_Proc_Define_Info obj_Proc_Define_Info;

	obj_XmlOpeation.Parse_XML_define_File("define.xml", obj_Proc_Define_Info);

	bool blState = obj_XmlOpeation.Parse_XML_File("protocol.xml", obj_Proc_Info);
	if(false == blState)
	{
		printf("[Main]Get XML(protocol.xml) error.\n");
		printf("[Main]please any key to exit.\n");
		getchar();
	}

	Create_Proc(obj_Proc_Define_Info, obj_Proc_Info);

	return 0;
}

