#include "XmlOpeation.h"

CXmlOpeation::CXmlOpeation(void)
{
	m_pTiXmlDocument = NULL;
	m_pRootElement   = NULL;
}

CXmlOpeation::~CXmlOpeation(void)
{
	Close();
}

bool CXmlOpeation::Init(const char* pFileName)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	return true;
}

bool CXmlOpeation::Init_String(const char* pXMLText)
{
	//解析字符串
	Close();
	m_pTiXmlDocument = new TiXmlDocument();
	m_pTiXmlDocument->Parse(pXMLText, 0, TIXML_DEFAULT_ENCODING); 

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	return true;
}

void CXmlOpeation::Close()
{
	if(NULL != m_pTiXmlDocument)
	{
		delete m_pTiXmlDocument;
		m_pTiXmlDocument = NULL;
		m_pRootElement   = NULL;
	}
}

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);
	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->Attribute(pAttrName);
	}

	return NULL;
}

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = NULL;
	if(NULL == pNextTiXmlElement)
	{
		pTiXmlElement = m_pRootElement->FirstChildElement(pName);
		pNextTiXmlElement = pTiXmlElement;
	}
	else
	{
		pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
		pNextTiXmlElement = pTiXmlElement;
	}

	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->Attribute(pAttrName);
	}

	return NULL;
}

char* CXmlOpeation::GetData_Text(const char* pName)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);
	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

char* CXmlOpeation::GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = NULL;
	if(NULL == pNextTiXmlElement)
	{
		pTiXmlElement = m_pRootElement->FirstChildElement(pName);
		pNextTiXmlElement = pTiXmlElement;
	}
	else
	{
		pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
		pNextTiXmlElement = pTiXmlElement;
	}

	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

bool CXmlOpeation::Parse_XML_Define_File(char* pFileName, _Proc_Define_Info& obj_Proc_Define_Info)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode     = NULL;

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//获得工程名称
	sprintf_safe(obj_Proc_Define_Info.m_szProcName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ProcName"));
	sprintf_safe(obj_Proc_Define_Info.m_szDBType, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("DB"));

	//循环获取预定义信息
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode = pMainNode->NextSiblingElement())
	{
		_Define_Info obj_Define_Info;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();
		if(strcmp("define", pMainElement->Value()) == 0)
		{
			sprintf_safe(obj_Define_Info.m_szSrcType, MAX_BUFF_50, "%s", pMainElement->Attribute("srcType"));
			sprintf_safe(obj_Define_Info.m_szTagType, MAX_BUFF_50, "%s", pMainElement->Attribute("tagType"));
			sprintf_safe(obj_Define_Info.m_szDesc, MAX_BUFF_100, "%s", pMainElement->Attribute("desc"));
			obj_Proc_Define_Info.obj_vec_Define_Info.push_back(obj_Define_Info);
		}
		else if(strcmp("enum", pMainElement->Value()) == 0)
		{
			_Enum_Info  obj_Enum_Info;
			TiXmlNode* pEnumNode     = NULL;
			sprintf_safe(obj_Enum_Info.m_szEnum, MAX_BUFF_50, "%s", pMainElement->Attribute("name"));

			//得到枚举字段
			for(pEnumNode = pMainNode->FirstChildElement();pEnumNode;pEnumNode=pEnumNode->NextSiblingElement())
			{
				_Enum_Name_Info obj_Enum_Name_Info;
				TiXmlElement* pEnumElement = pEnumNode->ToElement();

				sprintf_safe(obj_Enum_Name_Info.m_szEnumName, MAX_BUFF_50, "%s", pEnumElement->Attribute("name"));
				obj_Enum_Info.obj_vec_Enum_Name_Info.push_back(obj_Enum_Name_Info);
			}

			obj_Proc_Define_Info.obj_vec_Enum_Info.push_back(obj_Enum_Info);
		}
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
}

bool CXmlOpeation::Parse_XML_Table_File(const char* pFileName, _XML_Proc& obj_XML_Proc)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode     = NULL;
	TiXmlNode* pColumnNode   = NULL;

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//遍历所有子元素
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode=pMainNode->NextSiblingElement())
	{
		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		//获得子元素名称
		char sz_NodeName[MAX_BUFF_50] = {'\0'};
		sprintf_safe(sz_NodeName, MAX_BUFF_50, "%s", pMainNode->ToElement()->Value());

		if(strcmp("TableClass", sz_NodeName) == 0)
		{
			_Table_Info obj_Table_Info;
			
			if(NULL != pMainNode->ToElement()->Attribute("database"))
			{
				//如果是TableClass
				sprintf_safe(obj_Table_Info.m_sz_Db_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("database"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("table"))
			{
				sprintf_safe(obj_Table_Info.m_sz_Table_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("table"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("class"))
			{
				sprintf_safe(obj_Table_Info.m_sz_Class_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("class"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("classpool"))
			{
				obj_Table_Info.m_n_Class_Pool = atoi(pMainNode->ToElement()->Attribute("classpool"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("func"))
			{
				sprintf_safe(obj_Table_Info.m_sz_Func, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("func"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("SerialType"))
			{
				sprintf_safe(obj_Table_Info.m_sz_SerialType, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("SerialType"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("key"))
			{
				sprintf_safe(obj_Table_Info.m_sz_key, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("key"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("shmkey"))
			{
				sprintf_safe(obj_Table_Info.m_sz_ShmKey, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("shmkey"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("isdependfunc"))
			{
				obj_Table_Info.m_n_IsDependFunc = atoi(pMainNode->ToElement()->Attribute("isdependfunc"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("dependclass"))
			{
				sprintf_safe(obj_Table_Info.m_sz_DependClass, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("dependclass"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("desc"))
			{
				sprintf_safe(obj_Table_Info.m_sz_Desc, MAX_BUFF_100, "%s", pMainNode->ToElement()->Attribute("desc"));
			}
			if(NULL != pMainNode->ToElement()->Attribute("needlogicclass"))
			{
				obj_Table_Info.m_n_Need_Logic_Class = atoi(pMainNode->ToElement()->Attribute("needlogicclass"));
			}

			//遍历Column内容
			for(pColumnNode = pMainNode->FirstChildElement();pColumnNode;pColumnNode=pColumnNode->NextSiblingElement())
			{
				_Column_Info obj_Column_Info;

				int nColumnType = pColumnNode->Type();

				if(nColumnType != TiXmlText::TINYXML_ELEMENT)
				{
					continue;
				}

				TiXmlElement* pColumnElement = pColumnNode->ToElement();
				sprintf_safe(obj_Column_Info.m_sz_Column_Name, MAX_BUFF_50, "%s", pColumnElement->Attribute("name"));
				sprintf_safe(obj_Column_Info.m_sz_Db_Type, MAX_BUFF_50, "%s", pColumnElement->Attribute("dbtype"));
				sprintf_safe(obj_Column_Info.m_sz_Class_Type, MAX_BUFF_50, "%s", pColumnElement->Attribute("classtype"));
				if(NULL != pColumnElement->Attribute("length"))
				{
					obj_Column_Info.m_n_Length = atoi(pColumnElement->Attribute("length"));
				}
				if(NULL != pColumnElement->Attribute("value"))
				{
					sprintf_safe(obj_Column_Info.m_sz_Init_Value, MAX_BUFF_50, "%s", pColumnElement->Attribute("value"));
				}

				obj_Table_Info.m_obj_vec_Column_Info.push_back(obj_Column_Info);
			}

			//入结构体
			obj_XML_Proc.m_obj_vec_Table_Info.push_back(obj_Table_Info);

		}
		else if(strcmp("TableExt", sz_NodeName) == 0)
		{
			//如果是TableExt
			_Table_Ext obj_Table_Ext;

			sprintf_safe(obj_Table_Ext.m_sz_Db_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("database"));
			sprintf_safe(obj_Table_Ext.m_sz_Table_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("table"));
			sprintf_safe(obj_Table_Ext.m_sz_Class_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("class"));

			//遍历元素内容
			for(pColumnNode = pMainNode->FirstChildElement();pColumnNode;pColumnNode=pColumnNode->NextSiblingElement())
			{
				int nColumnType = pColumnNode->Type();
				if(nColumnType != TiXmlText::TINYXML_ELEMENT)
				{
					continue;
				}

				//判断变量过程
				if(strcmp(pColumnNode->ToElement()->Value(), "UnionKey") == 0)
				{
					sprintf_safe(obj_Table_Ext.m_sz_UnionKey, MAX_BUFF_50, "%s", pColumnNode->ToElement()->Attribute("value"));
				}
				else if(strcmp(pColumnNode->ToElement()->Value(), "Index") == 0)
				{
					_Table_Ext_Index obj_Table_Ext_Index;
					sprintf_safe(obj_Table_Ext_Index.m_sz_Value, MAX_BUFF_50, "%s", pColumnNode->ToElement()->Attribute("value"));
					sprintf_safe(obj_Table_Ext_Index.m_sz_Type, MAX_BUFF_50, "%s", pColumnNode->ToElement()->Attribute("type"));
					obj_Table_Ext.m_obj_vec_Table_Ext_Index.push_back(obj_Table_Ext_Index);
				}
				else if(strcmp(pColumnNode->ToElement()->Value(), "Sql") == 0)
				{
					_Table_Ext_SQL obj_Table_Ext_SQL;
					sprintf_safe(obj_Table_Ext_SQL.m_sz_Sql_Text, MAX_BUFF_100, "%s", pColumnNode->ToElement()->Attribute("value"));
					sprintf_safe(obj_Table_Ext_SQL.m_sz_Func_Name, MAX_BUFF_50, "%s", pColumnNode->ToElement()->Attribute("funcname"));
					obj_Table_Ext_SQL.m_n_Select_Type = atoi(pColumnNode->ToElement()->Attribute("selecttype"));
					sprintf_safe(obj_Table_Ext_SQL.m_sz_Input_Value, MAX_BUFF_100, "%s", pColumnNode->ToElement()->Attribute("inputvalue"));
					obj_Table_Ext.m_obj_vec_Table_Ext_SQL.push_back(obj_Table_Ext_SQL);
				}
			}

			obj_XML_Proc.m_obj_vec_Table_Ext.push_back(obj_Table_Ext);
		}
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;
	return true;
}

bool CXmlOpeation::Parse_XML_Table_Files(vec_Xml_File_Name obj_vec_Xml_File_Name, _XML_Proc& obj_XML_Proc)
{
	for(int i = 0; i < (int)obj_vec_Xml_File_Name.size(); i++)
	{
		//挨个读文件，填充数据结构
		Parse_XML_Table_File(obj_vec_Xml_File_Name[i].c_str(), obj_XML_Proc);
	}

	return true;
}

bool CXmlOpeation::Parse_XML_Test_Pool(char* pFileName, _PoolTest_Group& obj_PoolTest_Group)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode     = NULL;
	TiXmlNode* pColumnNode   = NULL;

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//获得工程名称
	sprintf_safe(obj_PoolTest_Group.m_sz_ProcName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ProcName"));

	//循环获取得测试对象池信息
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode = pMainNode->NextSiblingElement())
	{
		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		//获得子元素名称
		char sz_NodeName[MAX_BUFF_50] = {'\0'};
		sprintf_safe(sz_NodeName, MAX_BUFF_50, "%s", pMainNode->ToElement()->Value());

		if(strcmp("PoolTest", sz_NodeName) == 0)
		{
			_PoolTest_Info obj_PoolTest_Info;
			sprintf_safe(obj_PoolTest_Info.m_sz_Db_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("database"));
			sprintf_safe(obj_PoolTest_Info.m_sz_Table_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("table"));
			sprintf_safe(obj_PoolTest_Info.m_sz_Class_Name, MAX_BUFF_50, "%s", pMainNode->ToElement()->Attribute("class"));

			for(pColumnNode = pMainNode->FirstChildElement();pColumnNode;pColumnNode=pColumnNode->NextSiblingElement())
			{
				_PoolTest_Column obj_Column_Info;

				int nColumnType = pColumnNode->Type();

				if(nColumnType != TiXmlText::TINYXML_ELEMENT)
				{
					continue;
				}

				TiXmlElement* pColumnElement = pColumnNode->ToElement();
				sprintf_safe(obj_Column_Info.m_sz_Column_Name, MAX_BUFF_50, "%s", pColumnElement->Attribute("name"));
				sprintf_safe(obj_Column_Info.m_sz_Column_Type, MAX_BUFF_50, "%s", pColumnElement->Attribute("classtype"));
				if(NULL != pColumnElement->Attribute("length"))
				{
					obj_Column_Info.m_n_Colunm_Length = atoi(pColumnElement->Attribute("length"));
				}
				if(NULL != pColumnElement->Attribute("value"))
				{
					sprintf_safe(obj_Column_Info.m_sz_Column_Value, MAX_BUFF_50, "%s", pColumnElement->Attribute("value"));
				}
				if(NULL != pColumnElement->Attribute("iskey"))
				{
					obj_Column_Info.m_n_IsKey = atoi(pColumnElement->Attribute("iskey"));
				}

				obj_PoolTest_Info.m_vec_PoolTest_Column.push_back(obj_Column_Info);
			}
			obj_PoolTest_Group.m_vec_PoolTest_Info.push_back(obj_PoolTest_Info);
		}
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;
	return true;
}

bool CXmlOpeation::Parse_XML_DB_Server_File(char* pFileName, _DB_Server_Info& obj_DB_Server_Info)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode     = NULL;
	TiXmlNode* pColumnNode   = NULL;

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//获得工程名称
	sprintf_safe(obj_DB_Server_Info.m_sz_ProcName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ProcName"));

	TiXmlElement* pDBServerElement = m_pRootElement->FirstChildElement("DBServer");
	if(NULL != pDBServerElement)
	{
		TiXmlElement* pIntervalElement = pDBServerElement->FirstChildElement("Interval");
		if(NULL != pIntervalElement)
		{
			if(NULL != pIntervalElement->Attribute("time"))
			{
				obj_DB_Server_Info.m_DB_Server_Run_Info.m_n_Interval = atoi(pIntervalElement->Attribute("time"));
			}
		}

		TiXmlElement* pConfigElement = pDBServerElement->FirstChildElement("DBConfig");
		if(NULL != pConfigElement)
		{
			if(NULL != pConfigElement->Attribute("IP"))
			{
				sprintf_safe(obj_DB_Server_Info.m_DB_Server_Config.m_sz_DB_IP, MAX_BUFF_50, "%s", (char* )pConfigElement->Attribute("IP"));
			}
			if(NULL != pConfigElement->Attribute("DBName"))
			{
				sprintf_safe(obj_DB_Server_Info.m_DB_Server_Config.m_sz_DB_Name, MAX_BUFF_50, "%s", (char* )pConfigElement->Attribute("DBName"));
			}
			if(NULL != pConfigElement->Attribute("User"))
			{
				sprintf_safe(obj_DB_Server_Info.m_DB_Server_Config.m_sz_DB_User, MAX_BUFF_50, "%s", (char* )pConfigElement->Attribute("User"));
			}
			if(NULL != pConfigElement->Attribute("Pass"))
			{
				sprintf_safe(obj_DB_Server_Info.m_DB_Server_Config.m_sz_DB_Pass, MAX_BUFF_50, "%s", (char* )pConfigElement->Attribute("Pass"));
			}
		}

	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;
	return true;
}

