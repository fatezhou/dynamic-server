#pragma once
#include <string>
#include <vector>
class CDirWatcher
{
public:
	CDirWatcher();
	~CDirWatcher();

	enum{
		ACTION_ERRSTOP = -1,
		ACTION_ADDED = 0x00000001,
		ACTION_REMOVED = 0x00000002,
		ACTION_MODIFIED = 0x00000003,
		ACTION_RENAMED_OLD = 0x00000004,
		ACTION_RENAMED_NEW = 0x00000005
	};

	enum Filter
	{
		FILTER_FILE_NAME = 0x00000001, // add/remove/rename
		FILTER_DIR_NAME = 0x00000002, // add/remove/rename
		FILTER_ATTR_NAME = 0x00000004,
		FILTER_SIZE_NAME = 0x00000008,
		FILTER_LAST_WRITE_NAME = 0x00000010, // timestamp
		FILTER_LAST_ACCESS_NAME = 0x00000020, // timestamp
		FILTER_CREATION_NAME = 0x00000040, // timestamp
		FILTER_SECURITY_NAME = 0x00000100
	};

	virtual void OnChange(int nAction, const char* pFilePath) = 0;
	void Watch(const char* pDir);
};

