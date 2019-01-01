#include "DirWatcher.h"
#include <windows.h>


std::string ToString(const wchar_t* pSrc)
{
	std::string str;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, 0, 0, 0, 0);
	if (nLen <= 0){
		return "";
	}
	str.resize(nLen);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, (char*)str.data(), nLen, 0, 0);
	return str;
}

CDirWatcher::CDirWatcher()
{
}


CDirWatcher::~CDirWatcher()
{
}

void CDirWatcher::Watch(const char* pDir)
{
	HANDLE hHandle = CreateFileA(
		pDir,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (hHandle == INVALID_HANDLE_VALUE){
		OnChange(ACTION_ERRSTOP, pDir);
		return;
	}
	BYTE buf[2 * (sizeof(FILE_NOTIFY_INFORMATION) + 2 * MAX_PATH) + 2];
	FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION *)buf;
	DWORD dw;
	while (1){
		if (ReadDirectoryChangesW(
			hHandle,
			pNotify,
			sizeof(buf) - 2,
			true,
			FILTER_FILE_NAME | /*FILTER_DIR_NAME | */FILTER_LAST_WRITE_NAME,
			&dw,
			NULL,
			NULL
			)){
			for (FILE_NOTIFY_INFORMATION* p = pNotify; p;){
				//char c = p->FileName[p->FileNameLength / 2];
				p->FileName[p->FileNameLength / 2] = L'\0';
				OnChange(p->Action, ToString(p->FileName).c_str());
				if (p->NextEntryOffset)
					p = (PFILE_NOTIFY_INFORMATION)((BYTE*)p + p->NextEntryOffset);
				else
					p = 0;
			}
		}
		else{
			OnChange(ACTION_ERRSTOP, pDir);
		}
	}
}


