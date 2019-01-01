#pragma once
#include <vector>
#include <string>
#include <windows.h>

class IReporter
{
public:
	IReporter();
	~IReporter();
	virtual void Report(std::string strText){};
	virtual void AfterReport(){};
};

class CEmailReporter : public IReporter{
public:
	CEmailReporter();
	void Report(std::string strText);
	void SetUserInfo(const char* pUser, const char* pPassword, const char* pSmtp);
	void AddToReportAddr(const char* pEmailAddredss);
	void SetCmdAfterReport(const std::string& strCmd);
private:
	void AfterReport();
	std::vector<std::string> vecEmail;
	std::string strSMTP;
	std::string strUser;
	std::string strPassword;
	std::string strCmd;
};
