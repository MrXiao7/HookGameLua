#pragma once
#include "HookLua.h"
class CTLHookLua :
    public CHookLua
{
public:
    CTLHookLua();
    virtual bool InitLua();
    virtual ~CTLHookLua();

    //����Ҫ��ʾ��Ϣ���ڵ�ָ��
    void SetMainWnd(CWnd* pDlg){m_pMainDlg = pDlg;}
    CWnd * GetMainWnd(){ return m_pMainDlg; }
protected:
    CWnd *m_pMainDlg;//��ʾ��Ϣ���ڵ�ָ��
};

