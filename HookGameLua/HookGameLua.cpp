// HookGameLua.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HookGameLua.h"
#include "MainDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CHookGameLuaApp

BEGIN_MESSAGE_MAP(CHookGameLuaApp, CWinApp)
END_MESSAGE_MAP()


// CHookGameLuaApp ����

CHookGameLuaApp::CHookGameLuaApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHookGameLuaApp ����

CHookGameLuaApp theApp;
CMainDlg* pDlg;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    pDlg->DoModal();
    FreeLibraryAndExitThread(theApp.m_hInstance,777);
    return 0;
}

// CHookGameLuaApp ��ʼ��

BOOL CHookGameLuaApp::InitInstance()
{
	CWinApp::InitInstance();
    pDlg = new CMainDlg;
    ::CreateThread(NULL,NULL,ThreadProc,NULL,0,0);

	return TRUE;
}