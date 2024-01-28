#pragma once
#include "afxwin.h"
// CMainDlg 对话框
#include "resource.h"
#include "HookLua.h"

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DLGMAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
    // lua脚本内容
    CString m_csLuaText;
    CEdit m_editGameLua;
    CEdit m_editFilterate;
    HWND m_hGameWnd;
    CHookLua *m_pHookLua;
public:
    afx_msg void OnDestroy();
};
