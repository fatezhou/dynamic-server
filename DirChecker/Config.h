#pragma once

#include <string>
#include <vector>

using namespace std;

std::string GetDir();
vector<std::string> Split(string strSrc, string strDeli);

class Config
{
public:
	Config();
	~Config();

	void Load();
	std::string GetUser();
	std::string GetPassword();
	std::string GetDir();
	vector<std::string> GetEmails();
	vector<std::string> GetFliter();

private:
	std::string strUser, strPassword, strDir, strEmail, strFilterDir;
};

