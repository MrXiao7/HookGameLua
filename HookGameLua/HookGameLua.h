// HookGameLua.h : HookGameLua DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHookGameLuaApp
// �йش���ʵ�ֵ���Ϣ������� HookGameLua.cpp
//

class CHookGameLuaApp : public CWinApp
{
public:
	CHookGameLuaApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
