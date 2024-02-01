#pragma once
#include "HookLua.h"


typedef int (*FUNC_LUALOADBUFFER)(DWORD L, const char *buff, size_t sz, const char *name);
//call 0x0084F860
typedef int (*FUNC_LUAPCALL)(DWORD L, int nargs, int nresults, int errfunc);
//call 0x0084EC50

class CWowHookLua :
    public CHookLua
{
public:
    CWowHookLua();
    virtual ~CWowHookLua();
    virtual bool InitLua();
    virtual int LuaDostring(const char*);
private:
    FUNC_LUALOADBUFFER m_funluaLoadBuffer;
    FUNC_LUAPCALL m_funLuaPcall;
};

