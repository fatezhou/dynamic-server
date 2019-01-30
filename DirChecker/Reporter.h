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
	void AddToReportAddr(const char* pEmailAddredss);
	void SetCmdAfterReport(const std::string& strCmd);
	virtual void AfterReport();
protected:
	std::vector<std::string> vecEmail;
	std::string strCmd;
};

class CEmailReporter : public IReporter{
public:
	CEmailReporter();
	void Report(std::string strText);
	void SetUserInfo(const char* pUser, const char* pPassword, const char* pSmtp);
	void AddToReportAddr(const char* pEmailAddredss);
private:
	std::string strSMTP;
	std::string strUser;
	std::string strPassword;
};

class CCallSharpReport : public IReporter{
public:
	CCallSharpReport(){};
	void Report(std::string strText);

private:
	std::string strMail;
};
