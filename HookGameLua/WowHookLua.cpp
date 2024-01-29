#include "stdafx.h"
#include "WowHookLua.h"


CWowHookLua::CWowHookLua()
{
    _tcscpy_s(m_szClassName,_T("GxWindowClassD3d"));
}


CWowHookLua::~CWowHookLua()
{
}


bool CWowHookLua::InitLua()
{
    //HMODULE hGame = GetModuleHandle(_T("wow.exe"));
    m_funluaLoadBuffer = (FUNC_LUALOADBUFFER)0x0084F860;
    m_funLuaPcall = (FUNC_LUAPCALL)0x0084EC50;
    __asm
    {
        pushad;
        /*
        mov eax,dword ptr ds:[0xD3F78C];
        mov CHookLua::g_luaState, eax;
        */
        mov eax, 0x00817DB0;
        call eax;
        mov CHookLua::g_luaState, eax;
        popad;
    }
    //CHookLua::g_luaState = 0x1934A0F8;
    return true;
}

void Ansi2Utf8(LPCSTR szAnsi,LPSTR szUtf8)
{
    int wLen = MultiByteToWideChar(CP_ACP, 0, szAnsi, -1,NULL,0);
    LPWSTR pszW = (LPWSTR)_alloca((wLen + 1)*sizeof(WCHAR));

    //ansi×ª»»utf8
    MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, pszW, wLen);
    int aLen = WideCharToMultiByte(CP_UTF8,0,pszW,-1,NULL,0,NULL,NULL);
    WideCharToMultiByte(CP_UTF8, 0, pszW, -1, szUtf8, aLen,NULL,NULL);

    szUtf8[aLen] = 0;
    szUtf8[aLen+1] = 0;

}

int CWowHookLua::LuaDostring(const char* szBuffer)
{
    int nRet = 0;
    __try
    {
        //ansi-->utf-8
        unsigned int nLen = (strlen(szBuffer)+1)*2;
        //char *szUtf8Buff = (char*)malloc(nLen);
        char *szUtf8Buff = new char[nLen];
        Ansi2Utf8(szBuffer, szUtf8Buff);
        nRet = m_funluaLoadBuffer(CHookLua::g_luaState, szUtf8Buff, strlen(szUtf8Buff), szBuffer) || m_funLuaPcall(CHookLua::g_luaState, 0, -1, NULL);
        delete szUtf8Buff;
        szUtf8Buff = nullptr;
    }
    __except (1)
    {
        AfxMessageBox(_T("Ö´ÐÐLuaDostring³ö´í"));
    }

    return nRet;
}
