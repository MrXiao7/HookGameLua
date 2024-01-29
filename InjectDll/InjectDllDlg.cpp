
// InjectDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InjectDll.h"
#include "InjectDllDlg.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInjectDllDlg 对话框



CInjectDllDlg::CInjectDllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInjectDllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInjectDllDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTNREMOTE, &CInjectDllDlg::OnBnClickedBtnremote)
    ON_BN_CLICKED(IDC_BTNMSG, &CInjectDllDlg::OnBnClickedBtnmsg)
END_MESSAGE_MAP()


// CInjectDllDlg 消息处理程序

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInjectDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD GetProccessIdForName(const char* szName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    PROCESSENTRY32 processer;
    processer.dwSize = sizeof(PROCESSENTRY32);
    int flag = Process32First(hSnapshot,&processer);
    while (flag != 0)
    {
        _bstr_t processName(processer.szExeFile);//从TCHAR* 转为char*
        if (_stricmp(processName,szName) == 0)//strcmp不忽略大小写Game != game _stricmp会忽略大小写，Game==game
        {
            CloseHandle(hSnapshot);
            return processer.th32ProcessID;
        }
        flag = Process32Next(hSnapshot, &processer);
    }
    CloseHandle(hSnapshot);

    return -1;
}

void CInjectDllDlg::OnBnClickedBtnremote()
{
    char szDllPaht[] = "E:\\Projects\\MFC\\HookGameLua\\Debug\\HookGameLua.dll";
    int nPid = GetProccessIdForName("wow.exe");
    //如果等于-1就是没有找到进程
    HANDLE hGameProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,nPid);
    //判断是否打开成功
    //LoadLibraryA
    FARPROC func = GetProcAddress(GetModuleHandle(L"kernel32"),"LoadLibraryA");
    //判断是否是获取到函数地址
    LPVOID szRemoteDll =  VirtualAllocEx(hGameProcess, NULL, strlen(szDllPaht)+1,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
//     if (szRemoteDll ==0)
//     {
//     }
    //是否分配成功
    WriteProcessMemory(hGameProcess, szRemoteDll, szDllPaht, strlen(szDllPaht)+1,NULL);
    //是否写入成功
    HANDLE thread = CreateRemoteThread(hGameProcess, NULL, 0, (LPTHREAD_START_ROUTINE)func, szRemoteDll, 0, NULL);

    WaitForSingleObject(thread,INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(hGameProcess,szRemoteDll,strlen(szDllPaht)+1,MEM_DECOMMIT);
}


void CInjectDllDlg::OnBnClickedBtnmsg()
{
    //GameHook();
}
