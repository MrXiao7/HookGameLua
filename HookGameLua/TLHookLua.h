#pragma once
#include "HookLua.h"
class CTLHookLua :
    public CHookLua
{
public:
    CTLHookLua();
    virtual bool InitLua();
    virtual ~CTLHookLua();

    //设置要显示信息窗口的指针
    void SetMainWnd(CWnd* pDlg){m_pMainDlg = pDlg;}
    CWnd * GetMainWnd(){ return m_pMainDlg; }
protected:
    CWnd *m_pMainDlg;//显示信息窗口的指针
};

