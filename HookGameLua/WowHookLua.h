#pragma once
#include "HookLua.h"
class CWowHookLua :
    public CHookLua
{
public:
    CWowHookLua();
    virtual ~CWowHookLua();
    virtual bool InitLua();
};

