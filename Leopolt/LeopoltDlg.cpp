
// LeopoltDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Leopolt.h"
#include "LeopoltDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeopoltDlg �Ի���



CLeopoltDlg::CLeopoltDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLeopoltDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLeopoltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CLeopoltDlg::OnMonitor(void* ptr)
{
	CLeopoltDlg* pSelf = (CLeopoltDlg*)ptr;
	pSelf->Monitor();
}

void CLeopoltDlg::Monitor()
{
	bool bStart = false;
	char sz[1024] = "";
	GetModuleFileNameA(NULL, sz, 1024);
	char* p = strrchr(sz, '\\');
	*p = 0;
	strcat(sz, "\\iron man.exe");
	if (bStart){
		return;
	}
	while (1){
		bStart = true;
		STARTUPINFOA si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		if (!CreateProcessA(NULL, sz, NULL, NULL, false,
			0, NULL, NULL, &si, &pi)){
			MessageBoxA(m_hWnd, "no found process", "", MB_OK);
		}
		else{
			WaitForSingleObject(pi.hProcess, INFINITE);
			continue;
		}
	}
}

BEGIN_MESSAGE_MAP(CLeopoltDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLeopoltDlg::OnClickStartup)
	ON_BN_CLICKED(IDC_BUTTON2, &CLeopoltDlg::OnClickHide)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CLeopoltDlg ��Ϣ�������

BOOL CLeopoltDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	::RegisterHotKey(m_hWnd, 1, MOD_CONTROL | MOD_ALT, 'Z');

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLeopoltDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLeopoltDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLeopoltDlg::OnClickStartup()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnMonitor, this, 0, 0);
}


void CLeopoltDlg::OnClickHide()
{
	MessageBoxA(m_hWnd, "ctrl + alt + z", "", MB_OK);
	ShowWindow(SW_HIDE);
}


void CLeopoltDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nKey2 == 'Z'){
		ShowWindow(SW_SHOW);
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
