#include "DBOperationCreate.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

vector<string> split(const string& s, const string& seperator)
{
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while(i != s.size())
    {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;

        while(i != s.size() && flag == 0)
        {
            flag = 1;

            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;

        while(j != s.size() && flag == 0)
        {
            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            }

            if(flag == 0)
            {
                ++j;
            }
        }

        if(i != j)
        {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }

    return result;
}


void Check_DB_Include_File(_Table_Info obj_Class_Info, _XML_Proc& obj_XML_Proc, vec_Include_Info& obj_vec_Include_Info)
{
    obj_vec_Include_Info.clear();

    //遍历所有的属性
    for(int i = 0; i < (int)obj_Class_Info.m_obj_vec_Column_Info.size(); i++)
    {
        for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); j++)
        {
            if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Class_Type, obj_XML_Proc.m_obj_vec_Table_Info[j].m_sz_Class_Name)  == 0)
            {
                //比较是否已经存在
                bool blIsFind = false;

                for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
                {
                    if(strcmp(obj_Class_Info.m_obj_vec_Column_Info[i].m_sz_Class_Type, obj_vec_Include_Info[k].m_szInclude) == 0)
                    {
                        blIsFind = true;
                        break;
                    }
                }

                if(false == blIsFind)
                {
                    _Include_Info obj_Include_Info;
                    sprintf_safe(obj_Include_Info.m_szInclude, MAX_BUFF_100, "%s", obj_XML_Proc.m_obj_vec_Table_Info[j].m_sz_Class_Name);
                    obj_Include_Info.m_n_Need_Logic_Class = obj_XML_Proc.m_obj_vec_Table_Info[j].m_n_Need_Logic_Class;
                    obj_vec_Include_Info.push_back(obj_Include_Info);
                }

                break;
            }
        }
    }
}


void Create_DB_Environment(_XML_Proc& obj_XML_Proc)
{
    char szTempPath[MAX_BUFF_50]   = {'\0'};

    sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
    _mkdir(szTempPath);
#else
    mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

    //创建LuaIncode目录
    sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DBWrapper", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
    _mkdir(szTempPath);
#else
    mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

    //拷贝指定文件到目录中去
    char szTempFile[MAX_BUFF_100] = {'\0'};
    sprintf_safe(szTempFile, MAX_BUFF_100, "%s/conn_pool.h", szTempPath);
    Tranfile("../MysqlCommon/conn_pool.h", szTempFile);
    sprintf_safe(szTempFile, MAX_BUFF_100, "%s/conn_pool.cpp", szTempPath);
    Tranfile("../MysqlCommon/conn_pool.cpp", szTempFile);
    sprintf_safe(szTempFile, MAX_BUFF_100, "%s/mysql_encap.h", szTempPath);
    Tranfile("../MysqlCommon/mysql_encap.h", szTempFile);
    sprintf_safe(szTempFile, MAX_BUFF_100, "%s/mysql_encap.cpp", szTempPath);
    Tranfile("../MysqlCommon/mysql_encap.cpp", szTempFile);
    sprintf_safe(szTempFile, MAX_BUFF_100, "%s/lock.h", szTempPath);
    Tranfile("../MysqlCommon/lock.h", szTempFile);

    //创建数据库脚本目录
    sprintf_safe(szTempPath, MAX_BUFF_50, "%s/DBScript", obj_XML_Proc.m_sz_ProcName);
#ifdef WIN32
    _mkdir(szTempPath);
#else
    mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif
}

bool Create_DB_H(_XML_Proc& obj_XML_Proc)
{
    char szTemp[1024]     = {'\0'};
    char szPathFile[200]  = {'\0'};

    sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.h",
                 obj_XML_Proc.m_sz_ProcName);

    FILE* pFile = fopen(szPathFile, "wb");

    if(NULL == pFile)
    {
        return false;
    }

    sprintf_safe(szTemp, 200, "#ifndef _DB_OP_H_\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, 200, "#define _DB_OP_H_\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, 200, "#include \"Common_Define.h\"\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, 200, "#include \"conn_pool.h\"\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, 200, "#include \"mysql_encap.h\"\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

    //需要添加代码
    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    }

    sprintf_safe(szTemp, 200, "\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        char szSqlKey[MAX_BUFF_50] = {'\0'};
        To_Upper_String(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Func, szSqlKey);
        string strSqlKey = szSqlKey;

        if (0 == strSqlKey.length())
        {
            continue;
        }

        if( strSqlKey.find( "SELECT", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, 200, "bool select_%s(%s& obj);\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "INSERT", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, 200, "bool insert_%s(%s& obj);\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "UPDATE", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, 200, "bool update_%s(%s& obj);\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "DELETE", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, 200, "bool delete_%s(%s& obj);\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        sprintf_safe(szTemp, 200, "\n");
        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

        //printf("strSqlKey:%s\n", strSqlKey.c_str());
    }

    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Ext.size(); i++)
    {
        for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL.size(); j++)
        {
            sprintf_safe(szTemp, 200, "bool %s(%s);\n",
                         obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Func_Name,
                         obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Input_Value);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        sprintf_safe(szTemp, 200, "\n");
        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    }

    //为对象池创建加载数据方法 start
    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        //判断是否有Pool需要声明
        if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
        {
            //首先得到key的类型
            char szKeyType[MAX_BUFF_50] = {'\0'};

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
                {
                    sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                    break;
                }
            }

            //如果得不到Key类型，则不生成Pool代码
            if(strlen(szKeyType) == 0)
            {
                continue;
            }

            //这里判断是否有共享内存标签，如果有填充共享内存代码
            if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
            {
                sprintf_safe(szTemp, 200, "bool load_data_from_%s(%s* ptr%s, int poolsize);\n",
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                /*
                sprintf_safe(szTemp, 200, "bool save_data_to_%s(%s* ptr%s, int poolsize);\n",
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                */
                break;
            }
        }
    }

    //为对象池创建加载数据方法  end

    sprintf_safe(szTemp, 200, "#endif\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    fclose(pFile);
    return true;
}

bool Create_DB_CPP(_XML_Proc& obj_XML_Proc)
{
    char szTemp[10240]         = {'\0'};
    char szPathFile[200]      = {'\0'};
    char szSQL[MAX_BUFF_1024] = {'\0'};

    sprintf_safe(szPathFile, 200, "%s/DBWrapper/DB_Op.cpp",
                 obj_XML_Proc.m_sz_ProcName);

    FILE* pFile = fopen(szPathFile, "wb");

    if(NULL == pFile)
    {
        return false;
    }

    sprintf_safe(szTemp, sizeof(szTemp), "#include \"DB_Op.h\"\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, sizeof(szTemp), "#include <iostream>\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, sizeof(szTemp), "using namespace std;\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

    //需要添加代码
    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        //检查是否需要包含别的类对象的头文件
        vec_Include_Info obj_vec_Include_Info;
        Check_DB_Include_File((_Table_Info)obj_XML_Proc.m_obj_vec_Table_Info[i], obj_XML_Proc, obj_vec_Include_Info);

        char szSqlKey[MAX_BUFF_50] = {'\0'};
        To_Upper_String(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Func, szSqlKey);
        string strSqlKey = szSqlKey;

        if( strSqlKey.find( "SELECT", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, sizeof(szTemp), "bool select_%s(%s& obj)\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tchar szSql[1024000] = {0};\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tMysqlEncap* pme = ConnPool::GetInstance()->GetOneConn();\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (NULL == pme)\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"NULL == pme\"<<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tvector < map<string, string> > vmResultData;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) != 0)
                {
                    continue;
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"select * from %s.%s where %s = '%%s';\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        //浮点型
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"select * from %s.%s where %s = %%f;\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"select * from %s.%s where %s = %%d;\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"select * from %s.%s where %s = %%d;\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                }
            }

            //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tpme->ExecuteQuery(szSql,vmResultData);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (1 != vmResultData.size())\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"select error,sql:\"<< szSql <<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\telse\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(0 == strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key))
                {
                    continue;
                }

                if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "json") == 0)
                    {
                        bool bfindLogic = false;

                        for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
                        {

                            if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
                               (strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
                            {
                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s_Logic objjson;\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                bfindLogic = true;
                                break;
                            }
                        }

                        if (!bfindLogic)
                        {
                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s objjson;\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                    }
                    else
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(atoi((char*)vmResultData[0][\"%s\"].c_str()));\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(atof((char*)vmResultData[0][\"%s\"].c_str()));\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s((char*)vmResultData[0][\"%s\"].c_str());\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if (strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "string") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else
                        {
                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s_Logic objjson;\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                    }
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type, "json") == 0)
                    {
                        bool bfindLogic = false;

                        for(int k = 0; k < (int)obj_vec_Include_Info.size(); k++)
                        {

                            if((obj_vec_Include_Info[k].m_n_Need_Logic_Class == 1)&&
                               (strcmp(obj_vec_Include_Info[k].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type) == 0))
                            {
                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s_Logic objjson;\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                bfindLogic = true;
                                break;
                            }
                        }

                        if (!bfindLogic)
                        {
                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s objjson;\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                    }
                    else
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(atoi((char*)vmResultData[0][\"%s\"].c_str()));\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(atof((char*)vmResultData[0][\"%s\"].c_str()));\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s((char*)vmResultData[0][\"%s\"].c_str());\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else if (strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "string") == 0)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else
                        {
                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t%s_Logic objjson;\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.get_%s()->unserialization(vmResultData[0][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\tobj.set_%s(objjson);\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                    }
                }
            }

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "INSERT", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, sizeof(szTemp), "bool insert_%s(%s& obj)\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tchar szSql[1024000] = {0};\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tMysqlEncap* pme = ConnPool::GetInstance()->GetOneConn();\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (NULL == pme)\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"NULL == pme\"<<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            string strInsert = "insert into " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name) + "." + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name);

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if (0 == j)
                {
                    strInsert = strInsert + "(" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + ", ";
                }
                else if (j == (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size() -1 )
                {
                    strInsert = strInsert + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + ")";
                }
                else
                {
                    strInsert = strInsert + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + ", ";
                }
            }

            string strValue  = "value";
            string strObjValue  = "";

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if (0 == j)
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        strValue = strValue + "('%s', ";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        //如果是字符串，特殊处理
                        strValue = strValue + "(%d, ";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strValue = strValue + "(%f, ";
                    }
                    else
                    {
                        strValue = strValue + "('%s', ";
                    }

                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()->serialization().c_str(), ";
                    }
                }
                else if (j == (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size() -1 )
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        strValue = strValue + "'%s');";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        //如果是字符串，特殊处理
                        strValue = strValue + "%d);";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strValue = strValue + "%f);";
                    }
                    else
                    {
                        strValue = strValue + "'%s');";
                    }

                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()->serialization().c_str()";
                    }
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        strValue = strValue + "'%s', ";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strValue = strValue + "%d, ";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strValue = strValue + "%f, ";
                    }
                    else
                    {
                        strValue = strValue + "'%s', ";
                    }

                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()->serialization().c_str(), ";
                    }
                }
            }

            sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"%s %s\", %s);\n",
                         strInsert.c_str(),
                         strValue.c_str(),
                         strObjValue.c_str());
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (!pme->Execute(szSql))\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"insert error,sql:\"<< szSql <<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "UPDATE", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, sizeof(szTemp), "bool update_%s(%s& obj)\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tchar szSql[1024000] = {0};\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tMysqlEncap* pme = ConnPool::GetInstance()->GetOneConn();\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (NULL == pme)\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"NULL == pme\"<<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            string strUpdate = "update " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name) + "." + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name) + " set ";

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) == 0)
                {
                    continue;
                }
                else
                {
                    if (j == (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size() -1 )
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                        {
                            //如果是字符串，特殊处理
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "'%s'";
                        }
                        else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                        {
                            //如果是字符串，特殊处理
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "%d";
                        }
                        else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                        {
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "%f";
                        }
                        else
                        {
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "'%s'";
                        }
                    }
                    else
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                        {
                            //如果是字符串，特殊处理
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "'%s'" + ", ";
                        }
                        else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                        {
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "%d" + ", ";
                        }
                        else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                        {
                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "%f" + ", ";
                        }
                        else
                        {

                            strUpdate = strUpdate + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + " = " + "'%s'" + ", ";
                        }
                    }
                }
            }

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) != 0)
                {
                    continue;
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        strUpdate = strUpdate + " where " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) + " = " + "'%s';";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strUpdate = strUpdate + " where " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) + " = " + "%d;";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strUpdate = strUpdate + " where " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) + " = " + "%f;";
                    }
                    else
                    {
                        //如果是字符串，特殊处理
                        strUpdate = strUpdate + " where " + string(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) + " = " + "'%s';";
                    }
                }
            }

            string strObjValue  = "";

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) == 0)
                {
                    continue;
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "(), ";
                    }
                    else
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()->serialization().c_str(), ";
                    }
                }
            }

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) != 0)
                {
                    continue;
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "int") == 0)
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "double") == 0))
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()";
                    }
                    else
                    {
                        strObjValue = strObjValue + "obj.get_" + obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name + "()->serialization().c_str()";
                    }
                }
            }

            sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"%s\", %s);\n",
                         strUpdate.c_str(),
                         strObjValue.c_str());
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (!pme->Execute(szSql))\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"update error,sql:\"<< szSql <<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        if( strSqlKey.find( "DELETE", 0 ) != string::npos )
        {
            sprintf_safe(szTemp, sizeof(szTemp), "bool delete_%s(%s& obj)\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tchar szSql[1024000] = {0};\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tMysqlEncap* pme = ConnPool::GetInstance()->GetOneConn();\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (NULL == pme)\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"NULL == pme\"<<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) != 0)
                {
                    continue;
                }
                else
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type, "char") == 0)
                    {
                        //如果是字符串，特殊处理
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"delete from %s.%s where %s = '%%s';\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"delete from %s.%s where %s = %%d;\", obj.get_%s());\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                }
            }

            //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (!pme->Execute(szSql))\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"delete error,sql:\"<< szSql <<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }

        //printf("strSqlKey:%s\n", strSqlKey.c_str());
    }

    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Ext.size(); i++)
    {
        for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL.size(); j++)
        {
            sprintf_safe(szTemp, sizeof(szTemp), "bool %s(%s)\n",
                         obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Func_Name,
                         obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Input_Value);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
            sprintf_safe(szTemp, sizeof(szTemp), "\tchar szSql[1024000] = {0};\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tMysqlEncap* pme = ConnPool::GetInstance()->GetOneConn();\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\tif (NULL == pme)\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"NULL == pme\"<<endl;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            //扩展sql语句逻辑处理 start
            string strValue = "";
            string strReturnValue = "";
            vector<string> vecParas = split(obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Input_Value, ",");

            if (1==obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_n_Select_Type)
            {
                string strClass = "";

                for(vector<string>::size_type iLoop = 0; iLoop != vecParas.size(); ++iLoop)
                {
                    if (iLoop < vecParas.size() - 1)
                    {
                        vector<string> vecValues = split(vecParas[iLoop], " ");

                        if (iLoop < vecParas.size() - 2)
                        {
                            strValue = strValue + vecValues[1] + ", ";
                        }
                        else
                        {
                            strValue = strValue + vecValues[1];
                        }
                    }
                    else
                    {
                        vector<string> vecValues = split(vecParas[iLoop], "&");
                        strReturnValue = vecValues[1];

                        strClass = split(split(vecValues[0], "<")[1], ">")[0];
                    }
                }

                //printf("strValue:%s\n",strValue.c_str());
                //printf("strReturnValue:%s\n",strReturnValue.c_str());
                //printf("strClass:%s\n",strClass.c_str());

                strReturnValue.erase(0,strReturnValue.find_first_not_of(" "));
                strReturnValue.erase(strReturnValue.find_last_not_of(" ") + 1);

                sprintf_safe(szTemp, sizeof(szTemp), "\t%s.clear();\n",strReturnValue.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\tvector< map<string, string> > vmResultData;\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                if (strValue != "")
                {
                    sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"%s\", %s);\n", obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Sql_Text,strValue.c_str());
                    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                }
                else
                {
                    sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"%s\");\n",
                                 obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Sql_Text);
                    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                }

                //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\tif (!pme->ExecuteQuery(szSql,vmResultData))\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"record error\"<<endl;\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\telse\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\tfor(int iLoop = 0; iLoop < vmResultData.size(); iLoop++)\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\t{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s obj;\n",strClass.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                int iLoop = 0;
                vec_Include_Info obj_vec_Include_Info;

                for(iLoop = 0; iLoop < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); iLoop++)
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_sz_Class_Name, obj_XML_Proc.m_obj_vec_Table_Ext[i].m_sz_Class_Name) == 0)
                    {
                        //检查是否需要包含别的类对象的头文件
                        Check_DB_Include_File((_Table_Info)obj_XML_Proc.m_obj_vec_Table_Info[iLoop], obj_XML_Proc, obj_vec_Include_Info);
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                if (iLoop == obj_XML_Proc.m_obj_vec_Table_Info.size())
                {
                    printf("TableExt config error table:%s\n", obj_XML_Proc.m_obj_vec_Table_Ext[i].m_sz_Table_Name);
                }

                for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info.size(); k++)
                {
                    if(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_n_Length == 0)
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Db_Type, "json") == 0)
                        {
                            bool bfindLogic = false;

                            for(int kLoop = 0; kLoop < (int)obj_vec_Include_Info.size(); kLoop++)
                            {

                                if((obj_vec_Include_Info[kLoop].m_n_Need_Logic_Class == 1)&&
                                   (strcmp(obj_vec_Include_Info[kLoop].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type) == 0))
                                {
                                    //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s_Logic objjson;\n",
                                    //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type);
                                    //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                    sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.get_%s()->unserialization(vmResultData[iLoop][\"%s\"]);\n",
                                                 obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                                 obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                    //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(objjson);\n",
                                    //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                    //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                    bfindLogic = true;
                                    break;
                                }
                            }

                            if (!bfindLogic)
                            {
                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s objjson;\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.get_%s()->unserialization(vmResultData[iLoop][\"%s\"]);\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(objjson);\n",
                                //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                        }
                        else
                        {
                            if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type, "int") == 0)
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(atoi((char*)vmResultData[iLoop][\"%s\"].c_str()));\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                            else if((strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[k].m_sz_Class_Type, "float") == 0)||(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[k].m_sz_Class_Type, "double") == 0))
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(atof((char*)vmResultData[iLoop][\"%s\"].c_str()));\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                            else if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type, "char") == 0)
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s((char*)vmResultData[iLoop][\"%s\"].c_str());\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                            else if (strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type, "string") == 0)
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(vmResultData[iLoop][\"%s\"]);\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                            else
                            {
                                bool bfindLogic = false;

                                for(int kLoop = 0; kLoop < (int)obj_vec_Include_Info.size(); kLoop++)
                                {

                                    if((obj_vec_Include_Info[kLoop].m_n_Need_Logic_Class == 1)&&
                                       (strcmp(obj_vec_Include_Info[kLoop].m_szInclude, obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type) == 0))
                                    {
                                        //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s_Logic objjson;\n",
                                        //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type);
                                        //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                        sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.get_%s()->unserialization(vmResultData[iLoop][\"%s\"]);\n",
                                                     obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                                     obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                        //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(objjson);\n",
                                        //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                        //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                        bfindLogic = true;
                                        break;
                                    }
                                }

                                if (!bfindLogic)
                                {
                                    //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s objjson;\n",
                                    //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type);
                                    //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                    sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.get_%s()->unserialization(vmResultData[iLoop][\"%s\"]);\n",
                                                 obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                                 obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                    //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(objjson);\n",
                                    //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                    //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                                }
                            }
                        }
                    }
                    else
                    {
                        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Db_Type, "json") == 0)
                        {
                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s objjson;\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.get_%s()->unserialization(vmResultData[iLoop][\"%s\"]);\n",
                                         obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                         obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            //sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s(objjson);\n",
                            //  obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                            //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                        }
                        else
                        {
                            if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Class_Type, "char") == 0)
                            {
                                //如果是字符串，特殊处理
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s((char*)vmResultData[iLoop][\"%s\"].c_str());\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                            else
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tobj.set_%s((char*)vmResultData[iLoop][\"%s\"].c_str());\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name,
                                             obj_XML_Proc.m_obj_vec_Table_Info[iLoop].m_obj_vec_Column_Info[k].m_sz_Column_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                            }
                        }
                    }
                }

                sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t%s.push_back(obj);\n",strReturnValue.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\t}\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            }
            else
            {
                for(vector<string>::size_type iLoop = 0; iLoop != vecParas.size(); ++iLoop)
                {
                    if (iLoop < vecParas.size() - 2)
                    {
                        vector<string> vecValues = split(vecParas[iLoop], " ");

                        if (iLoop < vecParas.size() - 3)
                        {
                            strValue = strValue + vecValues[1] + ", ";
                        }
                        else
                        {
                            strValue = strValue + vecValues[1];
                        }
                    }
                }

                strReturnValue = split(obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Input_Value, "&")[1];

                //printf("strValue:%s\n",strValue.c_str());
                //printf("strReturnValue:%s\n",strReturnValue.c_str());

                strReturnValue.erase(0,strReturnValue.find_first_not_of(" "));
                strReturnValue.erase(strReturnValue.find_last_not_of(" ") + 1);

                sprintf_safe(szTemp, sizeof(szTemp), "\t%s.clear();\n",strReturnValue.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\tsprintf_common(szSql, sizeof(szSql), \"%s\", %s);\n", obj_XML_Proc.m_obj_vec_Table_Ext[i].m_obj_vec_Table_Ext_SQL[j].m_sz_Sql_Text,
                             strValue.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                //sprintf_safe(szTemp, sizeof(szTemp), "\tstring strSql = szSql;\n");
                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                //sprintf_safe(szTemp, 200, "\tstrSql = replace_all_distinct(strSql,\"\\\"\",\"\\\\\\\"\");\n");
                //fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\tif (!pme->ExecuteQuery(szSql,%s))\n",strReturnValue.c_str());
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"select error,sql:\"<< szSql <<endl;\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn false;\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
            }

            //扩展sql语句逻辑处理 end

            sprintf_safe(szTemp, sizeof(szTemp), "\tConnPool::GetInstance()->ReleaseOneConn(pme);\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            sprintf_safe(szTemp, sizeof(szTemp), "}\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }
    }

    //为对象池创建加载数据方法 start
    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        //判断是否有Pool需要声明
        if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
        {
            //首先得到key的类型
            char szKeyType[MAX_BUFF_50] = {'\0'};

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
                {
                    sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
                    break;
                }
            }

            //如果得不到Key类型，则不生成Pool代码
            if(strlen(szKeyType) == 0)
            {
                continue;
            }

            //这里判断是否有共享内存标签，如果有填充共享内存代码
            if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
            {
                sprintf_safe(szTemp, 200, "bool load_data_from_%s(%s* ptr%s, int poolsize)\n",
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, 200, "{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, 200, "\tvector<%s> vecObj;\n",obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Ext.size(); j++)
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Ext[j].m_sz_Class_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name) != 0)
                    {
                        continue;
                    }
                    else
                    {
                        for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Ext[j].m_obj_vec_Table_Ext_SQL.size(); k++)
                        {

                            if (1 == obj_XML_Proc.m_obj_vec_Table_Ext[j].m_obj_vec_Table_Ext_SQL[0].m_n_For_Pool)
                            {
                                sprintf_safe(szTemp, sizeof(szTemp), "\tbool bResult = %s(vecObj);\n",
                                             obj_XML_Proc.m_obj_vec_Table_Ext[j].m_obj_vec_Table_Ext_SQL[0].m_sz_Func_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\tif(!bResult)\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\tcout<<\"exec load_data_from_%s error\"<<endl;\n",obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\treturn bResult;\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\tint mixpoolsize = poolsize;\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\tif(vecObj.size() < mixpoolsize)\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\tmixpoolsize = vecObj.size();\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);


                                sprintf_safe(szTemp, sizeof(szTemp), "\tfor(int iLoop = 0; iLoop < mixpoolsize; ++iLoop)\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t\tptr%s[iLoop] = vecObj[iLoop];\n",
                                             obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                                sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
                                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                                break;
                            }
                            else
                            {
                                continue;
                            }
                        }

                        break;
                    }
                }

                sprintf_safe(szTemp, 200, "}\n\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                //test
                /*
                sprintf_safe(szTemp, 200, "bool save_data_to_%s(%s* ptr%s, int poolsize)\n",
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
                    obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                sprintf_safe(szTemp, 200, "{\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Ext.size(); j++)
                {
                    if(strcmp(obj_XML_Proc.m_obj_vec_Table_Ext[j].m_sz_Class_Name, obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name) != 0)
                    {
                        continue;
                    }
                    else
                    {
                        for(int k = 0; k < (int)obj_XML_Proc.m_obj_vec_Table_Ext[j].m_obj_vec_Table_Ext_SQL.size(); k++)
                        {
                            sprintf_safe(szTemp, sizeof(szTemp), "\tfor(int iLoop = 0; iLoop < poolsize; ++iLoop)\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t{\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\tif (ptr%s[iLoop].check_init())\n",
                                obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t{\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tbool bResult = update_%s(ptr%s[iLoop]);\n",
                                obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
                                obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\tif(!bResult)\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t{\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t\tcout<<\"exec save_data_to_%s error\"<<endl;\n",obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name);
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t\treturn bResult;\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t\t}\n\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t\t}\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\t}\n\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                            sprintf_safe(szTemp, sizeof(szTemp), "\treturn true;\n");
                            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

                        }

                        break;
                    }
                }

                sprintf_safe(szTemp, 200, "}\n\n");
                fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                */
                //test
            }
        }
    }

    //为对象池创建加载数据方法  end

    fclose(pFile);
    return true;
}

bool Create_DB_Script(_XML_Proc& obj_XML_Proc)
{
    //创建SQL表创建脚本语句
    char szTemp[1024]         = {'\0'};
    char szPathFile[200]      = {'\0'};
    char szSQL[MAX_BUFF_1024] = {'\0'};
    sprintf_safe(szPathFile, 200, "%s/DBScript/SQL_Script.sql",
                 obj_XML_Proc.m_sz_ProcName);
    FILE* pFile = fopen(szPathFile, "wb");

    if(NULL == pFile)
    {
        return false;
    }

    for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
    {
        if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_SerialType, "") == 0)
        {
            //如果是需要生成数据库表的
            sprintf_safe(szTemp, sizeof(szTemp), "CREATE TABLE %s.%s(\n",
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Db_Name,
                         obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
            {
                if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type) > 0)
                {
                    if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length == 0)
                    {
                        sprintf_safe(szTemp, sizeof(szTemp), "%s %s,\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szTemp, sizeof(szTemp), "%s %s(%d),\n",
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Db_Type,
                                     obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_n_Length);
                        fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
                    }
                }
            }

            sprintf_safe(szTemp, sizeof(szTemp), ");\n\n");
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
        }
    }

    fclose(pFile);
    return true;
}
void Create_DB_Proc(_XML_Proc& obj_XML_Proc)
{
    Create_DB_Environment(obj_XML_Proc);

    Create_DB_H(obj_XML_Proc);

    Create_DB_CPP(obj_XML_Proc);
    Create_DB_Script(obj_XML_Proc);
}
