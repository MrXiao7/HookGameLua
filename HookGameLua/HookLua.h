#pragma once
typedef int(*FUNC_LUADOSTRING)(DWORD, const char*);

class CHookLua
{
public:
    CHookLua();
    virtual ~CHookLua();
    //״̬��ָ��
    static DWORD g_luaState;

    //��ʼ��Lua ���麯�����̳е��������д�Լ���ȡlua�ӿڵ���ع���
    virtual bool InitLua()=0;

    //lua�ӿ���� lua_dostring
    virtual int LuaDostring(const char*);
    //int doFile
    
protected:
    FUNC_LUADOSTRING m_fun_luaDostring;
};
