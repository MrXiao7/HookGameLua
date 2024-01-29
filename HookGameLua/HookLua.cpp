#include "stdafx.h"
#include "HookLua.h"

DWORD CHookLua::g_luaState = -1;


CHookLua::CHookLua()
{
}


CHookLua::~CHookLua()
{
}


int CHookLua::LuaDostring(const char* szBuf)
{
    __try
    {
        return m_fun_luaDostring(CHookLua::g_luaState, szBuf);
    }
    __except (1)
    {
        AfxMessageBox(L"luaÖ´ÐÐdostring³ö´í");
    }

    return -1;
}

HWND CHookLua::GetGameHwnd()
{
    return ::FindWindow(m_szClassName, NULL);
}
