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

using namespace std;

#define MAX_BUFF_50   50
#define MAX_BUFF_100  100
#define MAX_BUFF_500  500
#define MAX_BUFF_1024 1024

//属性信息
struct _Property
{
	char          m_szPropertyName[MAX_BUFF_50];   //参数名称
	char          m_szDesc[MAX_BUFF_100];          //描述信息  
	char          m_szProperyType[MAX_BUFF_50];    //参数类型
	char          m_szInitValue[MAX_BUFF_50];      //参数初值 
	int           m_nLength;                       //长度 

	_Property()
	{
		m_szPropertyName[0] = '\0';
		m_szDesc[0]         = '\0';
		m_szProperyType[0]  = '\0';
		m_szInitValue[0]    = '\0';
		m_nLength           = 0;
	}
};

typedef vector<_Property> vecProperty;

//类信息
struct _Class_Info
{
	char m_szXMLName[60];
	char m_szDesc[MAX_BUFF_100];
	int  m_nPollCount;
	char m_szKey[MAX_BUFF_50];
	vecProperty m_vecProperty;

	_Class_Info()
	{
		m_szXMLName[0] = '\0';
		m_szDesc[0]    = '\0';
		m_szKey[0]     = '\0';
		m_nPollCount   = 0;
	}
};
typedef vector<_Class_Info> vec_Class_Info;

//工程名称
struct _Proc_Info
{
	char m_szProcName[MAX_BUFF_50];
	vec_Class_Info obj_vec_Class_Info;

	_Proc_Info()
	{
		m_szProcName[0] = '\0';
	}
};

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

struct _Proc_Define_Info
{
	char m_szProcName[MAX_BUFF_50];
	vec_Define_Info obj_vec_Define_Info;

	_Proc_Define_Info()
	{
		m_szProcName[0] = '\0';
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

//数据库操作相关
struct _DB_Column
{
	char m_szDBName[MAX_BUFF_50];
	char m_szDBType[MAX_BUFF_50];
	char m_szClassParam[MAX_BUFF_50];
	int  m_nIskey;

	_DB_Column()
	{
		m_nIskey          = 0;
		m_szDBName[0]     = '\0';
		m_szDBType[0]     = '\0';
		m_szClassParam[0] = '\0';
	}
};
typedef vector<_DB_Column> vec_DB_Column;

struct _DB_Table
{
	char m_szTableName[MAX_BUFF_50];
	char m_szClassName[MAX_BUFF_50];
	char m_szDBFunc[MAX_BUFF_100];
	vec_DB_Column m_vec_DB_Column;

	_DB_Table()
	{
		m_szTableName[0] = '\0';
		m_szClassName[0] = '\0';
		m_szDBFunc[0]    = '\0';
	}
};
typedef vector<_DB_Table> vec_DB_Table;

struct _DB_Proc
{
	char m_szProcName[MAX_BUFF_50];
	char m_szDBType[MAX_BUFF_50];
	vec_DB_Table m_vec_DB_Table;

	_DB_Proc()
	{
		m_szProcName[0] = '\0';
		m_szDBType[0]   = '\0';
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
