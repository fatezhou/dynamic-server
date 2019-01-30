#ifndef __DATABASE_API_H__
#define __DATABASE_API_H__
#include <vector>
#include <string>
#include "dbiConfig.h"
using TableString = std::vector<std::vector<std::string>>;
using Array = std::vector<std::string>;
using CSTR = const std::string;

//#define USE_MYSQL
//#define USE_SQLITE

namespace ZoyeeUtils{
	class IDatabaseAPI{
	public:
		virtual ~IDatabaseAPI(){};
		virtual bool Open(CSTR& strSrc, CSTR& strDatabase, CSTR& strUser, CSTR& strPassword) = 0;
		virtual void Close() = 0;
		virtual TableString Query(CSTR& strSQL) = 0;
		virtual int Exec(CSTR& strSQL) = 0;
	protected:
		bool bIsOpen = false;
	};

	enum SQL_TYPE{
#ifdef USE_MYSQL
		type_mysql,
#endif
#ifdef USE_SQLITE
		type_sqlite
#endif
	};

	IDatabaseAPI* CreateDatabaseAPI(SQL_TYPE type);
}

#endif