
// InjectDllDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CInjectDllDlg �Ի���



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


// CInjectDllDlg ��Ϣ�������

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInjectDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
        _bstr_t processName(processer.szExeFile);//��TCHAR* תΪchar*
        if (_stricmp(processName,szName) == 0)//strcmp�����Դ�СдGame != game _stricmp����Դ�Сд��Game==game
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
    //�������-1����û���ҵ�����
    HANDLE hGameProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,nPid);
    //�ж��Ƿ�򿪳ɹ�
    //LoadLibraryA
    FARPROC func = GetProcAddress(GetModuleHandle(L"kernel32"),"LoadLibraryA");
    //�ж��Ƿ��ǻ�ȡ��������ַ
    LPVOID szRemoteDll =  VirtualAllocEx(hGameProcess, NULL, strlen(szDllPaht)+1,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
//     if (szRemoteDll ==0)
//     {
//     }
    //�Ƿ����ɹ�
    WriteProcessMemory(hGameProcess, szRemoteDll, szDllPaht, strlen(szDllPaht)+1,NULL);
    //�Ƿ�д��ɹ�
    HANDLE thread = CreateRemoteThread(hGameProcess, NULL, 0, (LPTHREAD_START_ROUTINE)func, szRemoteDll, 0, NULL);

    WaitForSingleObject(thread,INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(hGameProcess,szRemoteDll,strlen(szDllPaht)+1,MEM_DECOMMIT);
}


void CInjectDllDlg::OnBnClickedBtnmsg()
{
    //GameHook();
}
