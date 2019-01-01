
// LeopoltDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Leopolt.h"
#include "LeopoltDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeopoltDlg 对话框



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


// CLeopoltDlg 消息处理程序

BOOL CLeopoltDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	::RegisterHotKey(m_hWnd, 1, MOD_CONTROL | MOD_ALT, 'Z');

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLeopoltDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLeopoltDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLeopoltDlg::OnClickStartup()
{
	// TODO:  在此添加控件通知处理程序代码
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnMonitor, this, 0, 0);
}


void CLeopoltDlg::OnClickHide()
{
	MessageBoxA(m_hWnd, "ctrl + alt + z", "", MB_OK);
	ShowWindow(SW_HIDE);
}


void CLeopoltDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nKey2 == 'Z'){
		ShowWindow(SW_SHOW);
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
