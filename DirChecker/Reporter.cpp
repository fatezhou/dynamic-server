#include "Reporter.h"
#include <iostream>
#pragma comment(lib, "ws2_32")

using namespace std;

extern std::string GetDir();

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
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout << "Open sockfd(TCP) error!" << endl;
		exit(-1);
	}
	if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
	{
		cout << "Connect sockfd(TCP) error!, errorCode:" << GetLastError() << endl;
		
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




void IReporter::AddToReportAddr(const char* pEmailAddredss)
{
	vecEmail.push_back(pEmailAddredss);
}

void IReporter::SetCmdAfterReport(const std::string& strCmd)
{
	this->strCmd = strCmd;
}

void IReporter::AfterReport()
{
	if (strCmd.empty()){
		return;
	}

	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	string strProcess = "cmd /c ";
	strProcess += GetDir().c_str();
	strProcess += "\\" + strCmd;

	CreateProcessA(NULL,
		(char*)strProcess.c_str(),
		NULL, NULL, false,
		0, NULL, NULL,
		&si,
		&pi
		);
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
	their_addr.sin_port = htons(587);
	hostent* hptr = gethostbyname(strSMTP.c_str());
	memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
	printf("%s", inet_ntoa(their_addr.sin_addr));
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
	sprintf_s(buf, 1500, "EHLO AO\r\n");
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
	sprintf_s(buf, 1500, "STARTTLS\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << buf << endl;

	// AUTH LOGIN
	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Auth Login Receive: " << rbuf << endl;

	memset(buf, 0, 1500);
	sprintf_s(buf, 1500, strUser.c_str());//��������˺�
	memset(login, 0, 128);
	EncodeBase64(login, buf, strlen(buf));
	sprintf_s(buf, 1500, "%s\r\n", login);
	send(sockfd, buf, strlen(buf), 0);
	cout << "Base64 UserName: " << buf << endl;
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "User Login Receive: " << rbuf << endl;

	// PASSWORD
	sprintf_s(buf, 1500, strPassword.c_str());//�����������
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
		// RCPT TO �ռ���
		sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", iter->c_str());
		send(sockfd, buf, strlen(buf), 0);
		memset(rbuf, 0, 1500);
		recv(sockfd, rbuf, 1500, 0);
		cout << "Tell Sendto Receive: " << rbuf << endl;
	}

	// DATA ׼����ʼ�����ʼ�����
	sprintf_s(buf, 1500, "DATA\r\n");
	send(sockfd, buf, strlen(buf), 0);
	memset(rbuf, 0, 1500);
	recv(sockfd, rbuf, 1500, 0);
	cout << "Send Mail Prepare Receive: " << rbuf << endl;


	string strBody;
	strBody += "From: ";
	strBody += "һλ���صĺ��ĵ�����";
	strBody += "\r\n";

	strBody += "To: ";
	strBody += " \r\n";

	strBody += "Subject: ";
	strBody += "�������ļ����޸���, ��ע��";
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

	//������
	closesocket(sockfd);

	AfterReport();
}

void CEmailReporter::SetUserInfo(const char* pUser, const char* pPassword, const char* pSmtp)
{
	strUser = pUser;
	strPassword = pPassword;
	strSMTP = pSmtp;
}

/*
Exchange
cjmlyiluvxpsbifg

IMAP
thbiagefszqobgii
*/

void CCallSharpReport::Report(std::string strText)
{
	printf("emails:%d", vecEmail.size());
	if (this->strMail.empty()){
		for (auto iter = vecEmail.begin(); iter != vecEmail.end(); iter++){
			string strOneAddr = "\"";
			strOneAddr += iter->c_str();
			strOneAddr += "\" ";
			this->strMail += strOneAddr;
		}
	}

	static string strPath;
	if (strPath.empty()){
		char sz[1024] = "";
		GetModuleFileNameA(NULL, sz, 1024);
		char* p = strrchr(sz, '\\');
		*p = 0;
		strcat(sz, "\\mail-sender.exe");
		strPath = sz;
	}

	string strCmd = strPath + " \"";
	strCmd += strText + "\" ";
	strCmd += strMail;

	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	printf("report cmd [%s]", strCmd.c_str());
	CreateProcessA(NULL,
		(char*)strCmd.c_str(),
		NULL, NULL, false,
		0, NULL, NULL,
		&si,
		&pi
		);
}
