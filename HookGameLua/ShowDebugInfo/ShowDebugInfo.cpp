#include "stdafx.h"
#include "ShowDebugInfo.h"

//#include <windows.h>
#include <tchar.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>

bool ShowDebugInfo()
{
    FILE* m_fpDebugOut = NULL;
    FILE* m_fpDebugIn = NULL;
    FILE* m_fpDebugErr = NULL;

    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        if (!AllocConsole())
        {
            FreeConsole();
            if (!AllocConsole())
            {
                return false;
            }
        }
    }
    SetConsoleTitle(_T("Game Debug... bpsend.net"));
    _tfreopen_s(&m_fpDebugOut, _T("CONOUT$"), _T("w"), stdout);
    _tfreopen_s(&m_fpDebugIn, _T("CONIN$"), _T("r"), stdin);
    _tfreopen_s(&m_fpDebugErr, _T("CONOUT$"), _T("w"), stderr);
    _tsetlocale(LC_ALL, _T("chs"));

    HANDLE  hStd;
    int     fd;
    FILE    *file;
    // 重定向标准输入流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_INPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "r");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdin = *file;

    // 重定向标准输出流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdout = *file;

    // 重定向标准错误流句柄到新的控制台窗口

    hStd = GetStdHandle(STD_ERROR_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stderr = *file;

    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hStd, &bInfo);
    COORD size = { 32767, 32767 };
    SetConsoleScreenBufferSize(hStd, size);

    _tsetlocale(LC_ALL, _T("chs"));
    printf("SetConsoleCP:936\n");
    SetConsoleCP(936);			//中文代码页
    SetConsoleOutputCP(936);

    //初始化Log4z
    ILog4zManager::getRef().start();
    return true;
}

bool CloseDebugInfo()
{
    ILog4zManager::getRef().stop();
    FreeConsole();
    

    return true;
}