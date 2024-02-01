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
        AfxMessageBox(L"lua执行dostring出错");
    }

    return -1;
}

HWND CHookLua::GetGameHwnd()
{
    return ::FindWindow(m_szClassName, NULL);
}

int CHookLua::LuaDoFile(const char* szPath)
{
    FILE *fLua;
    fopen_s(&fLua, szPath, "r");
    fseek(fLua, 0, SEEK_END);//把文件指针移动到文件尾
    long fileSize = ftell(fLua);
    char *szLua = new char[fileSize+2];
    memset(szLua, 0, fileSize + 2);
    //fseek(fLua, 0, 0);
    rewind(fLua);

    fread(szLua, 1, fileSize,fLua);
    szLua[fileSize] = '\0';
    int nRet = LuaDostring(szLua);
    
    delete szLua;
    fclose(fLua);

    return nRet;
}