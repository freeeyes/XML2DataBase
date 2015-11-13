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

bool CXmlOpeation::Parse_XML_Class_File(char* pFileName, _Proc_Info& obj_Proc_Info)
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

	TiXmlNode* pMainNode = NULL;
	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();
	
	//循环打印出每一个变量
	if(NULL == m_pRootElement)
	{
		return false;
	}
	sprintf_safe(obj_Proc_Info.m_szProcName, MAX_BUFF_50, "%s", m_pRootElement->ToElement()->Attribute("ProcName"));

	//获得根元素的子元素
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode=pMainNode->NextSiblingElement())
	{
		_Class_Info obj_Class_Info;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();

		//获得根元素的名称
		sprintf_safe(obj_Class_Info.m_szXMLName, MAX_BUFF_50, pMainElement->Value());
		sprintf_safe(obj_Class_Info.m_szDesc, MAX_BUFF_100, "%s", pMainElement->Attribute("desc"));
		char* pPoolCount = (char* )pMainElement->Attribute("PoolCount");
		if(NULL != pPoolCount)
		{
			obj_Class_Info.m_nPollCount =atoi(pPoolCount);
		}

		char* pKey = (char* )pMainElement->Attribute("key");
		if(NULL != pKey)
		{
			sprintf_safe(obj_Class_Info.m_szKey, MAX_BUFF_50, "%s", pKey);
		}

		//printf("Root=%s.\n", m_pRootElement->Value());
		//遍历类内容
		TiXmlNode* pNode = NULL;

		for(pNode = pMainElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
		{
			int nType = pNode->Type();
			if(nType == TiXmlText::TINYXML_ELEMENT)
			{
				//printf("Name=%s,Values=%s.\n", pNode->Value(), pNode->ToElement()->GetText());
				_Property objProperty;
				sprintf_safe(objProperty.m_szPropertyName, MAX_BUFF_50, "%s", pNode->Value());
				sprintf_safe(objProperty.m_szProperyType, MAX_BUFF_50, "%s", pNode->ToElement()->GetText());

				//同时获得字符串最大长度
				char* pLength = (char* )pNode->ToElement()->Attribute("length");
				if(NULL != pLength)
				{
					objProperty.m_nLength = (int)atoi(pLength);
				}

				char* pInitValue = (char* )pNode->ToElement()->Attribute("Value");
				if(NULL != pInitValue)
				{
					sprintf_safe(objProperty.m_szInitValue, MAX_BUFF_50, "%s", pInitValue);
				}
				
				sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

				obj_Class_Info.m_vecProperty.push_back(objProperty);
			}
		}

		obj_Proc_Info.obj_vec_Class_Info.push_back(obj_Class_Info);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
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
		sprintf_safe(obj_Define_Info.m_szSrcType, MAX_BUFF_50, "%s", pMainElement->Attribute("srcType"));
		sprintf_safe(obj_Define_Info.m_szTagType, MAX_BUFF_50, "%s", pMainElement->Attribute("tagType"));
		sprintf_safe(obj_Define_Info.m_szDesc, MAX_BUFF_100, "%s", pMainElement->Attribute("desc"));
		obj_Proc_Define_Info.obj_vec_Define_Info.push_back(obj_Define_Info);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
}

bool CXmlOpeation::Parse_XML_DB_File( char* pFileName, _DB_Proc& obj_DB_Proc )
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
	sprintf_safe(obj_DB_Proc.m_szProcName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ProcName"));
	sprintf_safe(obj_DB_Proc.m_szDBType, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("DB"));

	//目前先考虑只支持mysql
	if(strcmp("mysql", obj_DB_Proc.m_szDBType) != 0)
	{
		return false;
	}

	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode=pMainNode->NextSiblingElement())
	{
		_DB_Table obj_DB_Table;
		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();
		sprintf_safe(obj_DB_Table.m_szTableName, MAX_BUFF_50, "%s", pMainElement->Attribute("name"));
		sprintf_safe(obj_DB_Table.m_szClassName, MAX_BUFF_50, "%s", pMainElement->Attribute("class"));
		sprintf_safe(obj_DB_Table.m_szDBFunc, MAX_BUFF_100, "%s", pMainElement->Attribute("Func"));

		//循环遍历列内容
		for(pColumnNode = pMainElement->FirstChildElement();pColumnNode;pColumnNode=pColumnNode->NextSiblingElement())
		{
			_DB_Column obj_DB_Column;

			int nColumnType = pColumnNode->Type();

			if(nColumnType != TiXmlText::TINYXML_ELEMENT)
			{
				continue;
			}

			TiXmlElement* pColumnElement = pColumnNode->ToElement();
			sprintf_safe(obj_DB_Column.m_szDBName, MAX_BUFF_50, "%s", pColumnElement->Attribute("name"));
			sprintf_safe(obj_DB_Column.m_szDBType, MAX_BUFF_50, "%s", pColumnElement->Attribute("type"));
			sprintf_safe(obj_DB_Column.m_szClassParam, MAX_BUFF_100, "%s", pColumnElement->Attribute("classparam"));
			if(NULL != pColumnElement->Attribute("IsKey"))
			{
				obj_DB_Column.m_nIskey = atoi(pColumnElement->Attribute("IsKey"));
			}

			obj_DB_Table.m_vec_DB_Column.push_back(obj_DB_Column);

		}

		obj_DB_Proc.m_vec_DB_Table.push_back(obj_DB_Table);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
}