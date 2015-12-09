#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Common.h"

//将XML转换为MAP结构
//add by freeeyes


class CXmlOpeation
{
public:
  CXmlOpeation(void);
  ~CXmlOpeation(void);

  bool Init(const char* pFileName);
  bool Init_String(const char* pXMLText);

  char* GetData(const char* pName, const char* pAttrName);
  char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

  char* GetData_Text(const char* pName);
  char* GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement);

  bool Parse_XML_Define_File(char* pFileName, _Proc_Define_Info& obj_Proc_Define_Info);
  bool Parse_XML_Table_File(const char* pFileName, _XML_Proc& obj_XML_Proc);
  bool Parse_XML_Table_Files(vec_Xml_File_Name obj_vec_Xml_File_Name, _XML_Proc& obj_XML_Proc);
  bool Parse_XML_Test_Pool(char* pFileName, _PoolTest_Group& obj_PoolTest_Group);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif
