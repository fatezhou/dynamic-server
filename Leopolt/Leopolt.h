
// Leopolt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLeopoltApp: 
// �йش����ʵ�֣������ Leopolt.cpp
//

class CLeopoltApp : public CWinApp
{
public:
	CLeopoltApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLeopoltApp theApp;