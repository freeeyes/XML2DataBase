#ifndef _COMMON_H
#define _COMMON_H

//公共结构放在这里
//add by freeeyes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <ctype.h>
#include<iostream>
#include<fstream>
#include <string>

using namespace std;

#define MAX_BUFF_50   50
#define MAX_BUFF_100  100
#define MAX_BUFF_500  500
#define MAX_BUFF_1024 1024

//预定义信息
struct _Define_Info
{
	char m_szSrcType[MAX_BUFF_50];
	char m_szTagType[MAX_BUFF_50];
	char m_szDesc[MAX_BUFF_100];

	_Define_Info()
	{
		m_szSrcType[0]   = '\0';
		m_szTagType[0]   = '\0';
		m_szDesc[0]      = '\0';
	}
};
typedef vector<_Define_Info> vec_Define_Info;

struct _Enum_Name_Info
{
	char m_szEnumName[MAX_BUFF_50];

	_Enum_Name_Info()
	{
		m_szEnumName[0] = '\0';
	}
};
typedef vector<_Enum_Name_Info> vec_Enum_Name_Info;

struct _Enum_Info
{
	char m_szEnum[MAX_BUFF_50];
	vec_Enum_Name_Info obj_vec_Enum_Name_Info;

	_Enum_Info()
	{
		m_szEnum[0] = '\0';
	}
};
typedef vector<_Enum_Info> vec_Enum_Info;

struct _Proc_Define_Info
{
	char m_szProcName[MAX_BUFF_50];
	char m_szDBType[MAX_BUFF_50];
	vec_Define_Info obj_vec_Define_Info;
	vec_Enum_Info   obj_vec_Enum_Info;

	_Proc_Define_Info()
	{
		m_szProcName[0] = '\0';
		m_szDBType[0]   = '\0';
	}
};

struct _Include_Info
{
	char m_szInclude[MAX_BUFF_100];

	_Include_Info()
	{
		m_szInclude[0] = '\0';
	}
};
typedef vector<_Include_Info> vec_Include_Info;


//**************************************************
//新版数据结构
typedef vector<string> vec_Xml_File_Name;

struct _Column_Info
{
	char m_sz_Column_Name[MAX_BUFF_50];
	char m_sz_Db_Type[MAX_BUFF_50];
	char m_sz_Class_Type[MAX_BUFF_50];
	char m_sz_Desc[MAX_BUFF_100];   
	char m_sz_Init_Value[MAX_BUFF_50];
	int  m_n_Length;

	_Column_Info()
	{
		m_sz_Column_Name[0]  = '\0';
		m_sz_Db_Type[0]      = '\0';
		m_sz_Class_Type[0]   = '\0';
		m_sz_Desc[0]         = '\0';
		m_sz_Init_Value[0]   = '\0';
		m_n_Length           = 0;
	}
};
typedef vector<_Column_Info> vec_Column_Info;

struct _Table_Info
{
	char m_sz_Table_Name[MAX_BUFF_50];
	char m_sz_Db_Name[MAX_BUFF_50];
	char m_sz_Class_Name[MAX_BUFF_50];
	char m_sz_Func[MAX_BUFF_50];
	char m_sz_SerialType[MAX_BUFF_50];
	char m_sz_Desc[MAX_BUFF_100];
	char m_sz_key[MAX_BUFF_50];
	char m_sz_ShmKey[MAX_BUFF_50];
	char m_sz_DependClass[MAX_BUFF_50];
	int  m_n_IsDependFunc;
	int  m_n_Class_Pool;
	vec_Column_Info m_obj_vec_Column_Info;

	_Table_Info()
	{
		m_sz_Table_Name[0]  	 = '\0';
		m_sz_Db_Name[0]     	 = '\0';
		m_sz_Class_Name[0]  	 = '\0';
		m_sz_Func[0]        	 = '\0';
		m_sz_SerialType[0]  	 = '\0';
		m_sz_Desc[0]        	 = '\0';
		m_sz_key[0]         	 = '\0';
		m_sz_ShmKey[0]      	 = '\0';
		m_sz_DependClass[0] 	 = '\0';
		m_n_IsDependFunc       	 = 0;
		m_n_Class_Pool      	 = 0;
	}
};
typedef vector<_Table_Info> vec_Table_Info;

struct _Table_Ext_Index
{
	char m_sz_Value[MAX_BUFF_50];
	char m_sz_Type[MAX_BUFF_50];

	_Table_Ext_Index()
	{
		m_sz_Value[0]    = '\0';
		m_sz_Type[0]     = '\0';
	}
};
typedef vector<_Table_Ext_Index> vec_Table_Ext_Index;

struct _Table_Ext_SQL
{
	char m_sz_Sql_Text[MAX_BUFF_100];
	char m_sz_Func_Name[MAX_BUFF_50];
	char m_sz_Input_Value[MAX_BUFF_100];
	int  m_n_Select_Type;

	_Table_Ext_SQL()
	{
		m_sz_Sql_Text[0]     = '\0';
		m_sz_Func_Name[0]    = '\0';
		m_sz_Input_Value[0]  = '\0';
		m_n_Select_Type = 0;
	}
};
typedef vector<_Table_Ext_SQL> vec_Table_Ext_SQL;

struct _Table_Ext
{
	char m_sz_Table_Name[MAX_BUFF_50];
	char m_sz_Db_Name[MAX_BUFF_50];
	char m_sz_Class_Name[MAX_BUFF_50];
	char m_sz_UnionKey[MAX_BUFF_50];
	
	vec_Table_Ext_Index m_obj_vec_Table_Ext_Index;
	vec_Table_Ext_SQL m_obj_vec_Table_Ext_SQL;

	_Table_Ext()
	{
		m_sz_Table_Name[0]    = '\0';
		m_sz_Db_Name[0]       = '\0';
		m_sz_Class_Name[0]    = '\0';
		m_sz_UnionKey[0]      = '\0';
	}
};
typedef vector<_Table_Ext> vec_Table_Ext;

struct _XML_Proc
{
	char              m_sz_ProcName[MAX_BUFF_50];
	char              m_sz_DBType[MAX_BUFF_50];
	vec_Table_Info    m_obj_vec_Table_Info;
	vec_Table_Ext     m_obj_vec_Table_Ext;

	_XML_Proc()
	{
		m_sz_ProcName[0] = '\0';
		m_sz_DBType[0]   = '\0';
	}
};

//**************************************************

//对象池测试相关类型
struct _PoolTest_Column
{
	char m_sz_Column_Name[MAX_BUFF_50];
	char m_sz_Column_Type[MAX_BUFF_50];
	int  m_n_Colunm_Length;
	char m_sz_Column_Value[MAX_BUFF_50];
	int  m_n_IsKey;

	_PoolTest_Column()
	{
		m_sz_Column_Name[0]    = '\0';
		m_sz_Column_Type[0]    = '\0';
		m_sz_Column_Value[0]   = '\0';
		m_n_Colunm_Length      = 0;
		m_n_IsKey              = 0;
	}
};
typedef vector<_PoolTest_Column> vec_PoolTest_Column;

struct _PoolTest_Info
{
	char m_sz_Table_Name[MAX_BUFF_50];
	char m_sz_Db_Name[MAX_BUFF_50];
	char m_sz_Class_Name[MAX_BUFF_50];
	vec_PoolTest_Column m_vec_PoolTest_Column;

	_PoolTest_Info()
	{
		m_sz_Table_Name[0] = '\0';
		m_sz_Db_Name[0] = '\0';
		m_sz_Class_Name[0] = '\0';
	}
};
typedef vector<_PoolTest_Info> vec_PoolTest_Info;


struct _PoolTest_Group
{
	char              m_sz_ProcName[MAX_BUFF_50];
	vec_PoolTest_Info m_vec_PoolTest_Info;

	_PoolTest_Group()
	{
		m_sz_ProcName[0] = '\0';
	}
};

static void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(szText, nLen, fmt, ap);
	szText[nLen - 1] = '\0';

	va_end(ap);
};

static void To_Upper_String(const char* pSrc, char* pRet)
{
	int nLen = (int)strlen(pSrc);

	for(int i = 0; i < nLen; i++)
	{
		pRet[i] = toupper(pSrc[i]);
	}
	pRet[nLen] = '\0';
}

static void Tranfile(const char* pFileSrc, const char* pFileDes)
{
	fstream fsCopee( pFileSrc, ios::binary | ios::in ) ;
	fstream fsCoper( pFileDes, ios::binary | ios::out ) ;
	fsCoper << fsCopee.rdbuf() ;
}

#endif
