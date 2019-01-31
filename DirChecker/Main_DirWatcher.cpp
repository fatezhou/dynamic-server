#include "DirWatcher.h"
#include <queue>
#include <vector>
#include <string>
#include <windows.h>
#include "Reporter.h"
#include "Config.h"
#include "DatabaseAPI.h"

const char* pPayText = "支付码可能被修改";
const char* pDbConnectFail = "数据库可能连不上";

Config cfg;
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
			int nMaxSize = 10;
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
			pReporter->Report(strText);
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

void TestSQL(){
	ZoyeeUtils::IDatabaseAPI* db = ZoyeeUtils::CreateDatabaseAPI(ZoyeeUtils::type_mysql);
	db->Open("127.0.0.1:3307", "we7", "we7", "123456aa");
	TableString table = db->Query("select appid from ims_zh_jdgjb_system where id = 1");
	for (int i = 0; i < table.size(); i++){
		printf("[%s]\n", table[i][0].c_str());
	}
};

class CDataBaseChecker{
public:
	static void Run(void* pDataBaseChecker){
		CDataBaseChecker* pThis = (CDataBaseChecker*)pDataBaseChecker;
		pThis->DoChecker();
	};

	void DoChecker(){
		db = ZoyeeUtils::CreateDatabaseAPI(ZoyeeUtils::type_mysql);
		pCallSharpReport = new CCallSharpReport;
		vector<string> vecEmails = cfg.GetEmails();
		char sz[1024] = { 0 };
		for (auto iter = vecEmails.begin(); iter != vecEmails.end(); iter++){
			printf("%s\n", iter->c_str());
			pCallSharpReport->AddToReportAddr(iter->c_str());			
		}
		if (db->Open("127.0.0.1:3307", "we7", "we7", "123456aa") == false){
			sprintf(sz, "[%s]%s", GetTimeStr().c_str(), pDbConnectFail);
			pCallSharpReport->Report(sz);
			return;
		}
		printf("connect db ok\n");
		//G54gDF5r54dsfg45DS4DF45FSdf465FS
		string strDbPayCode;
		string strRightPayCode = "G54gDF5r54dsfg45DS4DF45FSdf465FS";
		const char* pSql = "select wxkey from ims_zh_jdgjb_system where id = 1";
		while (1){
			Sleep(5000);
			TableString table = db->Query(pSql);
			if (table.size() < 1){
				continue;
			}
			strDbPayCode = table[0][0].c_str();			
			printf("[%s]%s", GetTimeStr().c_str(), strDbPayCode.c_str());
			if (strDbPayCode == strRightPayCode){
				continue;
			}
			sprintf(sz, "[%s]%s", GetTimeStr().c_str(), pPayText);
			pCallSharpReport->Report(sz);
		}
	}

	void Start(){
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Run, this, 0, 0);
	};
	ZoyeeUtils::IDatabaseAPI* db;
	CCallSharpReport* pCallSharpReport;
};

int main(){

	cfg.Load();

	CCallSharpReport* pCallSharpReport = new CCallSharpReport;
	vector<string> vecEmails = cfg.GetEmails();
	for (auto iter = vecEmails.begin(); iter != vecEmails.end(); iter++){
		printf("%s\n", iter->c_str());
		pCallSharpReport->AddToReportAddr(iter->c_str());
	}	
	CDataBaseChecker* pDbChecker = new CDataBaseChecker;
	pDbChecker->Start();
	CMyWatcher* pWatcher = new CMyWatcher;
	pWatcher->SetReporter(pCallSharpReport);
	pWatcher->Watch(cfg.GetDir().c_str());
	return 0;		 
}