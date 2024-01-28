#pragma once
#include "afxwin.h"
// CMainDlg �Ի���
#include "resource.h"
#include "HookLua.h"

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_DLGMAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnExeclua();
    afx_msg void OnBnClickedBtnFiterate();
    afx_msg void OnBnClickedBtnClearfiterate();
    afx_msg void OnBnClickedBtnCleargamelua();
    void AddLuaList(CString strText);
    void GameMsgProc(WPARAM,LPARAM);
    HWND GetGameHwnd();
private:
    // lua�ű�����
    CString m_csLuaText;
    CEdit m_editGameLua;
    CEdit m_editFilterate;
    HWND m_hGameWnd;
    CHookLua *m_pHookLua;
public:
    afx_msg void OnDestroy();
};
