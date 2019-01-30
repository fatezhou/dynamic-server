#ifndef __MY_SQL_API_H__
#define __MY_SQL_API_H__
#include "DatabaseAPI.h"

#ifdef USE_MYSQL
#include "mysql/include/mysql.h"

namespace ZoyeeUtils{
	class CMysqlAPI : public IDatabaseAPI{
	public:
		~CMysqlAPI();
		virtual bool Open(CSTR& strSrc, CSTR& strDatabase, CSTR& strUser, CSTR& strPassword);
		virtual void Close();
		virtual TableString Query(CSTR& strSQL);
		virtual int Exec(CSTR& strSQL);

	private:
		MYSQL m_mysql;
	};
}
#endif
#endif