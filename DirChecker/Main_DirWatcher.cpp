#include "DirWatcher.h"
#include <queue>
#include <vector>
#include <string>
#include <windows.h>
#include "Reporter.h"
#include "Config.h"


Config cfg;

class CMyWatcher : public CDirWatcher{
public:
	CMyWatcher() : CDirWatcher(){
		::InitializeCriticalSection(&cs);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ReportMonitorThread, this, 0, 0);
	}

	void SetReporter(IReporter* pReporter){
		this->pReporter = pReporter;
	};

	virtual void OnChange(int nAction, const char* pFilePath){
		char szBuffer[1024] = "";
		switch (nAction){
		case ACTION_ADDED:
			sprintf(szBuffer, "<div><h3>%s[新增文件]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		case ACTION_REMOVED:
			sprintf(szBuffer, "<div><h3>%s[删除文件]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		case ACTION_MODIFIED:
			sprintf(szBuffer, "<div><h3>%s[修改文件]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		case ACTION_RENAMED_OLD:
			sprintf(szBuffer, "<div><h3>%s[文件重命名(旧)]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		case ACTION_RENAMED_NEW:
			sprintf(szBuffer, "<div><h3>%s[文件重命名(新)]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		default:
			sprintf(szBuffer, "<div><h3>%s[其他错误]: %s</h3></div>", GetTimeStr().c_str(), pFilePath);
			break;
		}
		printf("%s\n", szBuffer);
		vector<string> vecFliter = cfg.GetFliter();		
		for (auto iter = vecFliter.begin(); iter != vecFliter.end(); iter++){
			if (strstr(pFilePath, iter->c_str()) != NULL){
				printf("在过滤行列, 不处理");
				return;
			}
		}
		Push(szBuffer);
	};

	void ToReport(){
		::EnterCriticalSection(&cs);
		if (bShouldBeReport){
			bShouldBeReport = false; 
			int nSize = 0;
			int nMaxSize = 50;
			nSize = vec.size();
			int nDeleteCount = nSize - nMaxSize;
			auto iter = vec.begin();
			while (nDeleteCount > 0){
				iter = vec.erase(iter);
				nDeleteCount--;
			}

			std::string strText;
			nSize = vec.size();
			for (int i = 0; i < nSize; i++){
				strText += vec[i];
			}
			//pReporter->Report(strText);
		}
		::LeaveCriticalSection(&cs);
	};

	static void ReportMonitorThread(void* ptr){
		CMyWatcher* pSelf = (CMyWatcher*)ptr;
		while (1){
			Sleep(5 * 1000);
			pSelf->ToReport();
		}
	};

	std::string GetTimeStr(){
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		char szTime[128] = "";
		sprintf(szTime, "[%04d/%02d/%02d][%02d:%02d:%02d]",
			sysTime.wYear,
			sysTime.wMonth,
			sysTime.wDay,
			sysTime.wHour,
			sysTime.wMinute,
			sysTime.wSecond
			);
		return szTime;
	};

private:
	void Push(const char* pText){
		::EnterCriticalSection(&cs);
		vec.push_back(pText); 
		bShouldBeReport = true; 
		::LeaveCriticalSection(&cs);
	};

	CRITICAL_SECTION cs;
	std::vector<std::string> vec;
	bool bShouldBeReport = false;
	IReporter* pReporter;
};

int main(){
	cfg.Load();
	cfg.GetEmails();
	CEmailReporter* pEmailReport = new CEmailReporter;
	//pEmailReport->SetUserInfo("81758624@qq.com", "hoqaduzkavthbhcb", "smtp.qq.com");
	pEmailReport->SetUserInfo(cfg.GetUser().c_str(), cfg.GetPassword().c_str(), cfg.GetSMTP().c_str());
	vector<string> vecEmails = cfg.GetEmails();
	for (auto iter = vecEmails.begin(); iter != vecEmails.end(); iter++){
		pEmailReport->AddToReportAddr(iter->c_str());
	}
/*
	pEmailReport->AddToReportAddr("927136570@qq.com");
	pEmailReport->AddToReportAddr("1490533119@qq.com");
	pEmailReport->AddToReportAddr("516039216@qq.com");
	*///pEmailReport->Report("<h1>这是一封测试用的邮件</h1>");
	CMyWatcher* pWatcher = new CMyWatcher;
	pWatcher->SetReporter(pEmailReport);
	pWatcher->Watch(cfg.GetDir().c_str());
	return system("pause");		 
}