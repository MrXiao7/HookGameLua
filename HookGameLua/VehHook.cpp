#include "stdafx.h"
#include "VehHook.h"

#pragma  warning(disable:4996)
void VehWriteMemCode(DWORD Adress, DWORD ArgAdress, DWORD ArgLen)
{
	DWORD OldPro;
	VirtualProtect((LPVOID)Adress, ArgLen, PAGE_EXECUTE_READWRITE, &OldPro);

	_try{
		memcpy((void*)Adress, (void*)ArgAdress, ArgLen);
	}
	_except(1)
	{

	}

	VirtualProtect((LPVOID)Adress, ArgLen, OldPro, NULL);
}
LONG NTAPI veh_optimization(struct _EXCEPTION_POINTERS * ExceptionInfo)
{

	DWORD Addr = (DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress;
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		CVehHook::GetInstance()->BreakRun(Addr, ExceptionInfo);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
	{
		CVehHook::GetInstance()->BreakOne(Addr);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_EXECUTE_HANDLER;
}


CVehHook* CVehHook::GetInstance()
{
	static CVehHook instance;
	return &instance;
}



void CVehHook::AddVeh(char * VehName, char *MoudleName, DWORD MoudleOffset, DWORD VehHookAdress, DWORD HookCodeLen_, int IsOnce, void(*Fun)(struct _EXCEPTION_POINTERS * ExceptionInfo))
{
	VehData Data;

	strcpy(Data.VehName_, VehName);

	if (MoudleName != NULL)
		strcpy(Data.MoudleName_, MoudleName);
	else
		memset(Data.MoudleName_, 0, 512);

	Data.MoudleOffset_ = MoudleOffset;
	Data.VehHookAdress_ = VehHookAdress;
	Data.HookCodeLen_ = HookCodeLen_;
	Data.IsOnce_ = IsOnce;
	Data.Fun_ = Fun;
	m_vehList.push_back(Data);

}
void CVehHook::BreakAll()
{
	m_cs.Lock();
	std::vector<VehData>::iterator It = m_vehList.begin();

	for (; It != m_vehList.end(); It++)
	{
		if (It->MoudleName_[0] != 0)
		{
			It->VehHookAdress_ = (DWORD)GetModuleHandleA(It->MoudleName_) + It->MoudleOffset_;
		}

		It->OldCode_ = *(BYTE*)It->VehHookAdress_;
		DWORD SetCodeData = 0xcc;
		VehWriteMemCode(It->VehHookAdress_, (DWORD)&SetCodeData, 1);

	}
	m_cs.Unlock();
}

void CVehHook::BreakOne(DWORD CodeAdress)
{
	m_cs.Lock();
    std::vector<VehData>::iterator It = m_vehList.begin();

	for (; It != m_vehList.end(); It++)
	{
	
		if (It->MoudleName_[0] != 0)
		{
			It->VehHookAdress_ = (DWORD)GetModuleHandleA(It->MoudleName_) + It->MoudleOffset_;
		}
		if (CodeAdress == It->VehHookAdress_ + It->HookCodeLen_)
		{
			DWORD SetCodeData = 0xcc;
			VehWriteMemCode(It->VehHookAdress_, (DWORD)&SetCodeData, 1);
			break;
		}


	}
	m_cs.Unlock();
}

void CVehHook::BreakRun(DWORD CodeAdress, _EXCEPTION_POINTERS * ExceptionInfo)
{
	m_cs.Lock();
    std::vector<VehData>::iterator It = m_vehList.begin();

	for (; It != m_vehList.end(); It++)
	{
		if (It->MoudleName_[0] != 0)
		{
			It->VehHookAdress_ = (DWORD)GetModuleHandleA(It->MoudleName_) + It->MoudleOffset_;
		}
		if (CodeAdress == It->VehHookAdress_)
		{
			VehWriteMemCode(It->VehHookAdress_, (DWORD)&It->OldCode_, 1);

			if (It->IsOnce_ == 0)
			{
				ExceptionInfo->ContextRecord->EFlags |= 0x100;
			}

			if (It->Fun_ != NULL)
			{
				It->Fun_(ExceptionInfo);
			}

			break;
		}
		

	}
	m_cs.Unlock();
}
void CVehHook::InitVeh()
{
	m_Handle = AddVectoredExceptionHandler(0, (PVECTORED_EXCEPTION_HANDLER)veh_optimization);

	BreakAll();
}



void CVehHook::ExitVeh()
{
	m_cs.Lock();
	RemoveVectoredExceptionHandler(m_Handle);


    std::vector<VehData>::iterator It = m_vehList.begin();

	for (; It != m_vehList.end(); It++)
	{
		if (It->MoudleName_[0] != 0)
		{
			It->VehHookAdress_ = (DWORD)GetModuleHandleA(It->MoudleName_) + It->MoudleOffset_;
		}

		VehWriteMemCode(It->VehHookAdress_, (DWORD)&It->OldCode_, 1);
	}
	
	m_cs.Unlock();
}