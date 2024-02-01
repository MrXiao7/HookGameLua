// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HookGameLua.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "./ShowDebugInfo/ShowDebugInfo.h"
#include "WowHookLua.h"
#include "TLHookLua.h"
#include <string>
// CMainDlg 对话框
#define WM_USER_GAEMPROC WM_USER+1
#define GAME_MSG_DOSTRING 1
#define GAME_MSG_DOFILE 2

CMainDlg *g_pWnd = nullptr;
WNDPROC oldProc = nullptr;
LRESULT CALLBACK NewWindowProc(HWND hwnd,      // handle to window
    UINT uMsg,      // message identifier
    WPARAM wParam,  // first message parameter
    LPARAM lParam   // second message parameter
    );
IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
    , m_csLuaText(_T(""))
    , m_hGameWnd(NULL)
    , m_pHookLua(nullptr)
{
    m_pHookLua = new CWowHookLua;
    //m_pHookLua = new CTLHookLua;
    //INITDEBUGINFO();
}

CMainDlg::~CMainDlg()
{
    if (m_pHookLua)
    {
        delete m_pHookLua;
    }
    //FREEDEBUGINFO();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LUATEXT, m_csLuaText);
    DDX_Control(pDX, IDC_EDITGAMELUA, m_editGameLua);
    DDX_Control(pDX, IDC_EDITFILTERATE, m_editFilterate);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_EXECLUA, &CMainDlg::OnBnClickedBtnExeclua)
    ON_BN_CLICKED(IDC_BTN_FITERATE, &CMainDlg::OnBnClickedBtnFiterate)
    ON_BN_CLICKED(IDC_BTN_CLEARFITERATE, &CMainDlg::OnBnClickedBtnClearfiterate)
    ON_BN_CLICKED(IDC_BTN_CLEARGAMELUA, &CMainDlg::OnBnClickedBtnCleargamelua)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_EXECLUAFILE, &CMainDlg::OnBnClickedBtnExecluafile)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序



//窗口初始化的时候会执行
BOOL CMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    g_pWnd = this;
    //((CTLHookLua*)m_pHookLua)->SetMainWnd(this);
    m_pHookLua->InitLua();

    oldProc = (WNDPROC)SetWindowLongPtr(GetGameHwnd(), GWLP_WNDPROC, (LONG)NewWindowProc);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
/*
定时器回调函数
KillTimer()

*/

void CMainDlg::OnBnClickedBtnExeclua()
{

    //PostMessage 消息丢到你的消息队列里面以后 立马返回
    //SendMessage 消息放到消息队列里面，等待执行完毕后再返回
    //LOGFMTD("%s,%d",var,1);
    ::SendMessage(GetGameHwnd(), WM_USER_GAEMPROC, GAME_MSG_DOSTRING, 0);
}


void CMainDlg::OnBnClickedBtnFiterate()
{
    CString strGameLua;
    CString strFilterate;
    m_editGameLua.GetWindowText(strGameLua);
    m_editFilterate.GetWindowText(strFilterate);
    strFilterate += strGameLua;
    m_editFilterate.SetWindowText(strFilterate);
}


void CMainDlg::OnBnClickedBtnClearfiterate()
{
    m_editFilterate.SetWindowText(_T(""));
}


void CMainDlg::OnBnClickedBtnCleargamelua()
{
    m_editGameLua.SetWindowText(_T(""));
}

void CMainDlg::AddLuaList(CString strText)
{
    CString strFilterate;
    m_editFilterate.GetWindowText(strFilterate);
    if (strFilterate.Find(strText) != -1)
    {
        return;
    }
    CString strLua;
    m_editGameLua.GetWindowText(strLua);
    strLua += strText;
    m_editGameLua.SetWindowText(strLua);
}

LRESULT CALLBACK NewWindowProc(HWND hwnd,      // handle to window
    UINT uMsg,      // message identifier
    WPARAM wParam,  // first message parameter
    LPARAM lParam   // second message parameter
    )
{
    if (uMsg == WM_USER_GAEMPROC)
    {
        g_pWnd->GameMsgProc(wParam,lParam);
    }
    return CallWindowProc(oldProc,hwnd,uMsg,wParam,lParam);
}


void CMainDlg::GameMsgProc(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case GAME_MSG_DOSTRING:
    {
        GetDlgItemText(IDC_EDIT_LUATEXT, m_csLuaText);
        std::string strLuaText = CStringA(m_csLuaText);
        m_pHookLua->LuaDostring(strLuaText.c_str());
    }
    break;
    case GAME_MSG_DOFILE:
    {
        m_pHookLua->LuaDoFile("d:\\test.lua");
    }
    default:
        break;
    }
}

void CMainDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    SetWindowLongPtr(GetGameHwnd(), GWLP_WNDPROC, (LONG)oldProc);
}

HWND CMainDlg::GetGameHwnd()
{
    if (m_hGameWnd == NULL)
    {
        m_hGameWnd = m_pHookLua->GetGameHwnd();
    }
    return m_hGameWnd;
}

void CMainDlg::OnBnClickedBtnExecluafile()
{
    ::SendMessage(GetGameHwnd(), WM_USER_GAEMPROC, GAME_MSG_DOFILE, 0);
}
