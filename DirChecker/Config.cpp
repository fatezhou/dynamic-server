#include "Config.h"
#include <windows.h>


Config::Config()
{
}


Config::~Config()
{
}

void Config::Load()
{
	std::string strPath = ::GetDir() + "\\config.ini";
	char sz[1024] = "";
	GetPrivateProfileStringA("set", "user", "81758624@qq.com", sz, 1024, strPath.c_str());
	strUser = sz;

	GetPrivateProfileStringA("set", "password", "hoqaduzkavthbhcb", sz, 1024, strPath.c_str());
	strPassword = sz;

	GetPrivateProfileStringA("set", "dir", ".\\", sz, 1024, strPath.c_str());
	strDir = sz;

	GetPrivateProfileStringA("set", "emails", "", sz, 1024, strPath.c_str());
	strEmail = sz;

	GetPrivateProfileStringA("set", "fliter", "", sz, 1024, strPath.c_str());
	strFilterDir = sz;

	GetPrivateProfileStringA("set", "smtp", "smtp.qq.com", sz, 1024, strPath.c_str());
	strSMTP = sz;

	GetPrivateProfileStringA("set", "cmd", "", sz, 1024, strPath.c_str());
	strProcessCmd = sz;
}

std::string Config::GetUser()
{
	return strUser;
}

std::string Config::GetPassword()
{
	return strPassword;
}

std::string Config::GetDir()
{
	return strDir;
}

std::string Config::GetSMTP()
{
	return strSMTP;
}

vector<std::string> Config::GetEmails()
{
	return Split(strEmail, ";");
}

vector<std::string> Config::GetFliter()
{
	return Split(strFilterDir, ";");
}

std::string GetDir()
{
	static std::string strPath;
	if (strPath.empty()){
		char sz[1024] = "";
		GetModuleFileNameA(NULL, sz, 1024);
		char* ptr = strrchr(sz, '\\');
		if (ptr){
			*ptr = 0;
		}
		strPath = sz;
	}
	return strPath;
}

std::string Config::GetReportDoProcessCmd()
{
	return this->strProcessCmd;
}

vector<std::string> Split(string strSrc, string strDeli)
{
	vector<std::string> vec;
	int nPos = 0, nBegin = 0;
	while (nPos != -1){
		nPos = strSrc.find(strDeli, nBegin); 
		vec.push_back(strSrc.substr(nBegin, nPos - nBegin));
		nBegin = nPos + strDeli.length();
	}
	return vec;
}
