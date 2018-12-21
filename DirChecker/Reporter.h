#pragma once
#include <vector>
#include <string>
#include <windows.h>

class IReporter
{
public:
	IReporter();
	~IReporter();
	virtual void Report(std::string strText) = 0;
};

class CEmailReporter : public IReporter{
public:
	CEmailReporter();
	void Report(std::string strText);
	void SetUserInfo(const char* pUser, const char* pPassword, const char* pSmtp);
	void AddToReportAddr(const char* pEmailAddredss);

private:
	std::vector<std::string> vecEmail;
	std::string strSMTP;
	std::string strUser;
	std::string strPassword;
};
