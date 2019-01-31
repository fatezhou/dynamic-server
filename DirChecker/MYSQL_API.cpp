#include "MYSQL_API.h"
#ifdef USE_MYSQL
#pragma comment(lib, "mysql/lib/libmysql")

ZoyeeUtils::CMysqlAPI::~CMysqlAPI()
{
	Close();
}

bool ZoyeeUtils::CMysqlAPI::Open(CSTR& strSrc, CSTR& strDatabase, CSTR& strUser, CSTR& strPassword)
{
	if ( ! mysql_init(&m_mysql)){
		printf("mysql_init fail\n");
		return false;			 
	}
	char szIP[32];
	strcpy(szIP, strSrc.c_str());	
	char* p = strchr(szIP, ':');
	*p = 0;	
	int nPort = atoi(++p);
	if ( ! mysql_real_connect(&m_mysql, szIP, strUser.c_str(), strPassword.c_str(), strDatabase.c_str(), nPort, 0, 0)){
		printf("mysql_real_connect fail\n");
		return false;
	}
	printf("CMysqlAPI::Open ok\n");
	return (bIsOpen = true);
}

void ZoyeeUtils::CMysqlAPI::Close()
{
	if (bIsOpen){
		mysql_close(&m_mysql);
		bIsOpen = false;
	}	
}

TableString ZoyeeUtils::CMysqlAPI::Query(CSTR& strSQL)
{
	TableString table;
	MYSQL_RES* pMysqlRes = nullptr;
	do{
		if (mysql_real_query(&m_mysql, strSQL.c_str(), strSQL.length()) != 0)break;
		pMysqlRes = mysql_store_result(&m_mysql);
		if (pMysqlRes == nullptr)break;
		int nCols = mysql_num_fields(pMysqlRes);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(pMysqlRes)){
			Array vec;
			for (int i = 0; i < nCols; i++){
				vec.push_back(row[i] ? row[i] : "NULL");
			}
			table.push_back(vec);
		}
	} while (false);
	return table;	
}

int ZoyeeUtils::CMysqlAPI::Exec(CSTR& strSQL)
{
	if (mysql_real_query(&m_mysql, strSQL.c_str(), strSQL.length()) == 0){
		return mysql_affected_rows(&m_mysql);
	}
	return 0;
}
#endif