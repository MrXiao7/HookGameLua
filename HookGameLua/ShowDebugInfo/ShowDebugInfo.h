#ifndef __SHOWDEBUGINFO_H_INCLUDE__
#define __SHOWDEBUGINFO_H_INCLUDE__

#include "log4z.h"
using namespace zsummer::log4z;

bool ShowDebugInfo();
bool CloseDebugInfo();

#ifdef _DEBUG
#define INITDEBUGINFO()		ShowDebugInfo()
#define FREEDEBUGINFO()		CloseDebugInfo()
#else
#define INITDEBUGINFO()
#define FREEDEBUGINFO()	
#endif // DEBUG

/*

#ifdef _DEBUG
#define InitDebugInfo()		ShowDebugInfo()
#define LOGFMTD(format, ...) {\
	char szMsg[4096] = {0};\
	sprintf_s(szMsg, 4096, "[%s]%d:", __FILE__, __LINE__);\
	sprintf_s(&szMsg[strlen(szMsg)], 4096 - strlen(szMsg), format, ## __VA_ARGS__);\
	strcat_s(szMsg, "\r\n");\
	fprintf (stderr, szMsg);\
	OutputDebugStringA(szMsg);\
}

#else
#define InitDebugInfo()
#define LOGFMTD(format, ...)

#endif // DEBUG
*/

#endif