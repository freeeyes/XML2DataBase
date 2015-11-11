#ifndef MYSQL_ENCAP_H_
#define MYSQL_ENCAP_H_

#include <mysql.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

class MysqlEncap{
public:
	MysqlEncap();
	~MysqlEncap();
	bool Connect(const char *ip, const char *username, const char *password);
	void CloseConnect();
	bool ExecuteQuery(const char *sql, vector < map<string, string> >& vmResultData);
	bool Execute(const char *sql);
	bool StartTransaction();
	//修改为：当返回值0的时候，是ROLLBACK，1--COMMIT
	bool EndTransaction();
	void EscapeString(char *to, char *from);
	uint GetLastInsertId();
	int  SetOptions(enum mysql_option option, const char* arg);
	
private:
	bool ReConnect();
private:
	bool   isConnected;//判断是否连接
	bool   bCommit;//当用到事务时，该值用来选择是commit还是rollback	
	MYSQL  sql_conn;//数据库连接
	//保存连接信息，保证重连
    string m_strHostIp;
    string m_strUserName;
    string m_strPassWord;
};

#endif
