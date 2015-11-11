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

using namespace std;

#define MAX_BUFF_50  50
#define MAX_BUFF_100 100

//属性信息
struct _Property
{
	char          m_szPropertyName[MAX_BUFF_50];   //参数名称
	char          m_szDesc[MAX_BUFF_100];          //描述信息  
	char          m_szProperyType[MAX_BUFF_50];    //参数类型
	int           m_nLength;                       //长度 

	_Property()
	{
		m_szPropertyName[0] = '\0';
		m_szDesc[0]         = '\0';
		m_szProperyType[0]  = '\0';
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
	vecProperty m_vecProperty;

	_Class_Info()
	{
		m_szXMLName[0] = '\0';
		m_szDesc[0]    = '\0';
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

#endif
