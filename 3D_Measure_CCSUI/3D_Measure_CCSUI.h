// 3D_Measure_CCSUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy3D_Measure_CCSUIApp:
// �йش����ʵ�֣������ 3D_Measure_CCSUI.cpp
//

class CMy3D_Measure_CCSUIApp : public CWinApp
{
public:
	CMy3D_Measure_CCSUIApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy3D_Measure_CCSUIApp theApp;