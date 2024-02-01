#pragma once
typedef int(*FUNC_LUADOSTRING)(DWORD, const char*);

class CHookLua
{
public:
    CHookLua();
    virtual ~CHookLua();
    //状态机指针
    static DWORD g_luaState;

    //初始化Lua 纯虚函数，继承的类必须重写自己获取lua接口的相关功能
    virtual bool InitLua()=0;

    //lua接口相关 lua_dostring
    virtual int LuaDostring(const char*);
    //int doFile
    virtual int LuaDoFile(const char*);

    //返回游戏窗口类名
    HWND GetGameHwnd();
protected:
    FUNC_LUADOSTRING m_fun_luaDostring;
    TCHAR m_szClassName[256];
};

