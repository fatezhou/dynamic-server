#include "Reporter.h"
#include <iostream>
#pragma comment(lib, "ws2_32")

using namespace std;

struct Base64Date6
{
	unsigned int d4 : 6;
	unsigned int d3 : 6;
	unsigned int d2 : 6;
	unsigned int d1 : 6;
};

char ConvertToBase64(char   uc){
	if (uc < 26)
	{
		return 'A' + uc;
	}
	if (uc < 52)
	{
		return 'a' + (uc - 26);
	}
	if (uc < 62)
	{
		return '0' + (uc - 52);
	}
	if (uc == 62)
	{
		return '+';
	}
	return '/';
};
void EncodeBase64(char   *dbuf, char *buf128, int len){
	struct Base64Date6 *ddd = NULL;
	int           i = 0;
	char          buf[256] = { 0 };
	char          *tmp = NULL;
	char          cc = '\0';

	memset(buf, 0, 256);
	strcpy_s(buf, 256, buf128);
	for (i = 1; i <= len / 3; i++)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6 *)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
	}
	if (len % 3 == 1)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6 *)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = '=';
		dbuf[(i - 1) * 4 + 3] = '=';
	}
	if (len % 3 == 2)
	{
		tmp = buf + (i - 1) * 3;
		cc = tmp[2];
		tmp[2] = tmp[0];
		tmp[0] = cc;
		ddd = (struct Base64Date6 *)tmp;
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		dbuf[(i - 1) * 4 + 3] = '=';
	}
	return;
};
//void SendMail(char   *email, char *body);
int OpenSocket(struct sockaddr *addr){
	int sockfd = 0;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout << "Open sockfd(TCP) error!" << endl;
		exit(-1);
	}
	if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
	{
		cout << "Connect sockfd(TCP) error!" << endl;
		exit(-1);
	}
	return sockfd;
};

IReporter::IReporter()
{

}


IReporter::~IReporter()
{
}



CEmailReporter::CEmailReporter() : IReporter()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
}

void CEmailReporter::Report(std::string strText)
{
	int     sockfd = { 0 };
	char    buf[1500] = { 0 };
	char    rbuf[1500] = { 0 };
	char    login[128] = { 0 };
	char    pass[128] = { 0 };
	struct sockaddr_in their_addr = { 0 };
	memset(&their_addr, 0, sizeof(their_addr));

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(25);
	hostent* hptr = gethostbyname(strSMTP.c_str());
	memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);

	sockfd = OpenSocket((struct sockaddr *)&their_addr);
	memset(rbuf, 0, 1500);
	while (recv(sockfd, rbuf, 1500, 0) == 0)
	{
		cout << "reconnect..." << endl;
		Sleep(2);
		sockfd = OpenSocket((struct sockaddr *)&their_addr);
		memset(rbuf, 0, 1500);
	}

	cout << rbuf << endl;

	// EHLO
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "EHLO HYL-PC\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "EHLO REceive: " << rbuf << endl;

	// AUTH LOGIN
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Auth Login Receive: " << rbuf << endl;

	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, strUser.c_str());//你的邮箱账号
	memset(login, 0, 128);
	EncodeBase64(login, buf, strlen(buf));
	sprintf_s(buf, 1500, "%s\r\n", login);
	send(sockfd, buf, strlen(buf), 0);
	cout << "Base64 UserName: " << buf << endl;
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "User Login Receive: " << rbuf << endl;

	// PASSWORD
	sprintf_s(buf, 1500, strPassword.c_str());//你的邮箱密码
	memset(pass, 0, 128);
	EncodeBase64(pass, buf, strlen(buf));
	sprintf_s(buf, 1500, "%s\r\n", pass);
	send(sockfd, buf, strlen(buf), 0);
	cout << "Base64 Password: " << buf << endl;

	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Send Password Receive: " << rbuf << endl;

	// MAIL FROM
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "MAIL FROM: <%s>\r\n", strUser.c_str());
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "set Mail From Receive: " << rbuf << endl;

	sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", strUser.c_str());
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Tell Sendto Receive: " << rbuf << endl;

	for (auto iter = vecEmail.begin(); iter != vecEmail.end(); iter++){
		// RCPT TO 收件人
		sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", iter->c_str());
		send(sockfd, buf, strlen(buf), 0);
		memset(rbuf, 0, 1500);
		recv(sockfd, rbuf, 1500, 0);
		cout << "Tell Sendto Receive: " << rbuf << endl;
	}

	// DATA 准备开始发送邮件内容
	sprintf_s(buf, 1500, "DATA\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Send Mail Prepare Receive: " << rbuf << endl;


	string strBody;
	strBody += "From: ";
	strBody += "一位神秘的好心的老者";
	strBody += "\r\n";

	strBody += "To: ";
	strBody += " \r\n";

	strBody += "Subject: ";
	strBody += "可能有文件被修改了, 请注意";
	strBody += "\r\n";

	strBody += "MIME-Version: 1.0";
	strBody += "\r\n";
	strBody += "Content-Type: multipart/mixed;boundary=qwertyuiop";
	strBody += "\r\n";
	strBody += "\r\n";

	send(sockfd, strBody.c_str(), strBody.length(), 0);
	
	strBody = "--qwertyuiop\r\n";
	strBody += "Content-Type: text/html;";
	strBody += "charset=\"gb2312\"\r\n\r\n";
	strBody += strText.c_str();
	strBody += "\r\n.\r\n";

	send(sockfd, strBody.c_str(), strBody.length(), 0);

	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Send Mail Receive: " << rbuf << endl;

	// QUIT
	sprintf_s(buf, 1500, "QUIT\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Quit Receive: " << rbuf << endl;

	//清理工作
	closesocket(sockfd);
}

void CEmailReporter::SetUserInfo(const char* pUser, const char* pPassword, const char* pSmtp)
{
	strUser = pUser;
	strPassword = pPassword;
	strSMTP = pSmtp;
}

void CEmailReporter::AddToReportAddr(const char* pEmailAddredss)
{
	vecEmail.push_back(pEmailAddredss);
}
