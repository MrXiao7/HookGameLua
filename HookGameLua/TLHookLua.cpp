#include "stdafx.h"
#include "MainDlg.h"
#include "TLHookLua.h"
#include "VehHook.h"


static CTLHookLua* pTLHOOK = nullptr;
CTLHookLua::CTLHookLua()
{
    pTLHOOK = this;
}


CTLHookLua::~CTLHookLua()
{
}


void GetLuaFile(struct _EXCEPTION_POINTERS * ExceptionInfo)
{
    char* szLuaText = (char*)*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 0);//获取脚本内容
    std::string szLuaFileName = (char*)*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 8);//获取文件名
    int nPos = szLuaFileName.rfind("/");
    if (nPos == -1)
    {
        nPos = 0;
    }
    szLuaFileName = szLuaFileName.substr(nPos);//取出/后面一截的文件名
    //判断文件夹是否存在，如果不存在就创建
    std::string strFullPath = "D:\\TLBBLUA" + szLuaFileName;
    HANDLE hFile = CreateFileA(strFullPath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DWORD dwErr = GetLastError();
        CString strErr;
        strErr.Format(_T("%s:errcode:%d"), strFullPath.c_str(), dwErr);
        return;
    }
    DWORD dwByWritten = 0;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    WriteFile(hFile, szLuaText, strlen(szLuaText) + 1, &dwByWritten, NULL);
    CloseHandle(hFile);

}

void GetLuaFunction(struct _EXCEPTION_POINTERS * ExceptionInfo)
{
    char* szLuaText1 = (char*)(ExceptionInfo->ContextRecord->Ecx);
    char* szLuaText2 = (char*)(ExceptionInfo->ContextRecord->Ebx);

    CString strText = CString(szLuaText1) + _T("  ") + CString(szLuaText2) + _T("\r\n");

    if (CHookLua::g_luaState == -1)
    {
        CHookLua::g_luaState = *(DWORD*)(ExceptionInfo->ContextRecord->Eax);
    }
    ((CMainDlg*)pTLHOOK->GetMainWnd())->AddLuaList(strText);
}

bool CTLHookLua::InitLua()
{
    // 获取游戏lua_dostring D:\XTLBB\Bin\LuaPlus.dll
    HMODULE hLua = GetModuleHandle(_T("LuaPlus.dll"));
    if (hLua == NULL)
    {
        AfxMessageBox(_T("获取LuaPlus.dll失败"));
        return false;
    }
    m_fun_luaDostring = (FUNC_LUADOSTRING)GetProcAddress(hLua, "lua_dostring");
    if (m_fun_luaDostring == NULL)
    {
        return false;
    }
    //std::bin funciont
    CVehHook::GetInstance()->AddVeh("GetFile", "Game.exe", 0x4D771F, 0, 2, 0, GetLuaFile);
    //4D 6A94
    CVehHook::GetInstance()->AddVeh("GetFunction", "Game.exe", 0x4D7B44, 0, 2, 0, GetLuaFunction);
    CVehHook::GetInstance()->InitVeh();
    return true;
}