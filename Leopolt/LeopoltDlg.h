
// LeopoltDlg.h : 头文件
//

#pragma once


// CLeopoltDlg 对话框
class CLeopoltDlg : public CDialogEx
{
// 构造
public:
	CLeopoltDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LEOPOLT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static void OnMonitor(void* ptr);
	void Monitor();
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickStartup();
	afx_msg void OnClickHide();
	
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
