
// LeopoltDlg.h : ͷ�ļ�
//

#pragma once


// CLeopoltDlg �Ի���
class CLeopoltDlg : public CDialogEx
{
// ����
public:
	CLeopoltDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LEOPOLT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	static void OnMonitor(void* ptr);
	void Monitor();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickStartup();
	afx_msg void OnClickHide();
	
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
