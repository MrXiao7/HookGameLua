#pragma once
#include "afxmt.h"
#include <vector>


#define VEHADMIN CCLVEH::Instance()

struct VehData
{
	char VehName_[512];
	char MoudleName_[512];
	int   MoudleOffset_;
	DWORD VehHookAdress_;
	DWORD HookCodeLen_;
	BOOL  IsOnce_;
	int OldCode_;
	void(*Fun_)(struct _EXCEPTION_POINTERS * ExceptionInfo);
};


LONG NTAPI veh_optimization(struct _EXCEPTION_POINTERS * ExceptionInfo);


class CVehHook
{
public:
	static CVehHook* GetInstance();
	CVehHook::~CVehHook()
	{
		ExitVeh();
	}
public:
	std::vector<VehData> m_vehList;
	CCriticalSection m_cs;
	LPVOID m_Handle = 0;
public:
	void AddVeh(char * VehName, char *MoudleName, DWORD MoudleOffset, DWORD VehHookAdress, DWORD HookCodeLen_, int IsOnce, void(*Fun)(struct _EXCEPTION_POINTERS * ExceptionInfo));

public:
	void BreakAll();
	void BreakOne(DWORD CodeAdress);
	void InitVeh();
	void BreakRun(DWORD CodeAdress, _EXCEPTION_POINTERS * ExceptionInfo);
	void ExitVeh();
};

