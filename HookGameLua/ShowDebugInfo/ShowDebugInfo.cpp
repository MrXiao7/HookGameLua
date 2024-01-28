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
    // �ض����׼������������µĿ���̨����

    hStd = GetStdHandle(STD_INPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
    file = _fdopen(fd, "r");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stdin = *file;

    // �ض����׼�����������µĿ���̨����

    hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //�ı�ģʽ
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stdout = *file;

    // �ض����׼������������µĿ���̨����

    hStd = GetStdHandle(STD_ERROR_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stderr = *file;

    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hStd, &bInfo);
    COORD size = { 32767, 32767 };
    SetConsoleScreenBufferSize(hStd, size);

    _tsetlocale(LC_ALL, _T("chs"));
    printf("SetConsoleCP:936\n");
    SetConsoleCP(936);			//���Ĵ���ҳ
    SetConsoleOutputCP(936);

    //��ʼ��Log4z
    ILog4zManager::getRef().start();
    return true;
}

bool CloseDebugInfo()
{
    ILog4zManager::getRef().stop();
    FreeConsole();
    

    return true;
}