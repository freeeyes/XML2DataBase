#include "mysql_encap.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <assert.h>

using namespace std;

/*构造函数*/
MysqlEncap::MysqlEncap()
{
    isConnected = false;
    mysql_init(&sql_conn);
}

/*析构函数*/
MysqlEncap::~MysqlEncap()
{
    CloseConnect();
}

/****************************************************
* 建立数据库连接
* ip: 数据库主机ip
* username: 登录mysql用户名
* password: 密码
****************************************************/
bool MysqlEncap::Connect(const char* ip, const char* username, const
                         char* password)
{
    m_strHostIp = ip;
    m_strUserName = username;
    m_strPassWord = password;

    if(isConnected)
    {
        return true;
    }

    if(mysql_real_connect(&sql_conn, ip, username, password,
                          NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(&sql_conn));
        return false;
    }

    if (mysql_query(&sql_conn, "set names utf8;") != 0)
    {
        throw mysql_error(&sql_conn);
    }
	
	//add by freeeyes
	//设置mysql重连
	char value = 1;
	mysql_options(&sql_conn, MYSQL_OPT_RECONNECT, &value);

    isConnected = true;
    //printf("connect establish [user:%s]\n",username);
    return true;
}

/************************************************
* 关闭连接
************************************************/
void MysqlEncap::CloseConnect()
{
    mysql_close(&sql_conn);
    isConnected = false;
}

/************************************************
* 重新连接
************************************************/
bool MysqlEncap::ReConnect()
{
    CloseConnect();

    if(mysql_real_connect(&sql_conn, m_strHostIp.c_str(), m_strUserName.c_str(), m_strPassWord.c_str(),
                          NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr,"Reconnect:%s\n", mysql_error(&sql_conn));
        return false;
    }

    if (mysql_query(&sql_conn, "set names utf8;") != 0)
    {
        throw mysql_error(&sql_conn);
    }

    isConnected = true;
    return true;
}
/************************************************
* 检查连接，如果连接断开则重连
************************************************/


/************************************************
* 查询语句
************************************************/
bool MysqlEncap::ExecuteQuery(const char* sql, vector < map<string, string> >& vmResultData)
{
    //printf("sql:%s\n",sql);
    assert(sql != NULL);

    if(sql == NULL)
    {
        return true;
    }

    if(!isConnected)
    {
        fprintf(stderr,"connection was not established\n");
        bCommit = false;//如果有事务，出错不能提交
    }
	
	//添加mysql_ping(),保证连接存活
	mysql_ping(&sql_conn);

    try
    {
        if(mysql_real_query(&sql_conn, sql, strlen(sql)) != 0)
        {
            fprintf(stderr, "select query error:%s\n", mysql_error(&sql_conn));
            bCommit = false;
            return false;
        }

        vmResultData.clear();

        MYSQL_RES* res = mysql_store_result(&sql_conn);

        if (res == NULL)
        {
            if (mysql_errno(&sql_conn) != 0)
            {
                fprintf(stderr, "select query error:%s\n", mysql_error(&sql_conn));
                bCommit = false;
                return false;
            }
            else
            {
                return true;
            }
        }

        int num_fields = mysql_num_fields(res);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != NULL)
        {
            map<string, string> tmpresult;
            //unsigned long* lengths = mysql_fetch_lengths(res);

            for (int i = 0; i < num_fields; i++)
            {
                if (row[i] != NULL)
                {
                    tmpresult[fields[i].name] = row[i];
                }
                else
                {
                    tmpresult[fields[i].name] = "";
                }
            }

            vmResultData.push_back(tmpresult);
        }

        mysql_free_result(res);
        return true;
    }
    catch(...)
    {
        ReConnect();//这种情况是在，执行语句没有出错的情况下，抛出的异常，另外isConnected = true
        isConnected = true;
        bCommit = false;//如果有事务，出错不能提交
        return false;
    }

    return false;
}

bool MysqlEncap::Execute(const char* sql)
{
    //printf("sql:%s\n",sql);
    if(sql == NULL)
    {
        return true;
    }

    if(!isConnected)
    {
        fprintf(stderr,"connection was not established\n");
        bCommit = false;//如果有事务，出错不能提交
        return false;
    }

    try
    {
        if(mysql_real_query(&sql_conn, sql, strlen(sql)) != 0)
        {
            fprintf(stderr, "modify query error:%s\n",mysql_error(&sql_conn));
            bCommit = false;
            return false;
        }

        MYSQL_RES* res = mysql_store_result(&sql_conn);

        if (res == NULL)
        {
            if (mysql_errno(&sql_conn) != 0)
            {
                fprintf(stderr, "modify query error:%s\n",mysql_error(&sql_conn));
                bCommit = false;
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            //执行了查询操作
            fprintf(stderr, "modify query error:%s\n",mysql_error(&sql_conn));
            bCommit = false;
            return false;
        }

    }
    catch(...)
    {
        ReConnect();//这种情况是在，执行语句没有出错的情况下，抛出的异常，另外isConnected = true
        isConnected = true;
        bCommit = false;//如果有事务，出错不能提交
        return false;
    }

    return true;
}

bool MysqlEncap::StartTransaction()
{
    bCommit = true;
    return Execute("START TRANSACTION;");
}

bool MysqlEncap::EndTransaction()
{
    if(bCommit)
    {
        Execute("COMMIT;");
        return 1;
    }
    else
    {
        Execute("ROLLBACK;");
        return 0;
    }
}

/*************************************************
 * 转义字符串,避免特殊字符
 * make sure sizeof(to) >= 2*strlen(from)+1
 *************************************************/
void MysqlEncap::EscapeString(char* dest, char* source)
{
    mysql_real_escape_string(&sql_conn, dest, source, strlen(source));
}

uint MysqlEncap::GetLastInsertId()
{
    return (uint32_t)mysql_insert_id(&sql_conn);
}

int MysqlEncap::SetOptions(enum mysql_option option, const char* arg)
{
    return mysql_options(&sql_conn, option, arg);
}



