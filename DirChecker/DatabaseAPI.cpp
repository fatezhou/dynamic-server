#include "DatabaseAPI.h"

#ifdef USE_MYSQL
#include "MYSQL_API.h"
#endif

#ifdef USE_SQLITE
#include "SQLite_API.h"
#endif

ZoyeeUtils::IDatabaseAPI* ZoyeeUtils::CreateDatabaseAPI(SQL_TYPE type)
{
	IDatabaseAPI* pAPI = nullptr;
	switch (type)
	{
#ifdef USE_MYSQL
	case ZoyeeUtils::type_mysql:
		pAPI = new CMysqlAPI;	
		break;
#endif		
#ifdef USE_SQLITE
	case ZoyeeUtils::type_sqlite:
		pAPI = new CSQLiteAPI;
		break;
#endif		
	default:
		break;
	}
	return pAPI;
}
