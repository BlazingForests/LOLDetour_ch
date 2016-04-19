
// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
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


// CMainDlg 对话框




CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
	, EDIT_PATH_TEXT(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_PATH);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_InitLOLClient, &CMainDlg::OnBnClickedBtnInitlolclient)
	ON_BN_CLICKED(IDC_BTN_UninitLOLClient, &CMainDlg::OnBnClickedBtnUninitlolclient)
	ON_BN_CLICKED(IDC_BTN_InitLOL, &CMainDlg::OnBnClickedBtnInitlol)
	ON_BN_CLICKED(IDC_BTN_UninitLOL, &CMainDlg::OnBnClickedBtnUninitlol)
	ON_BN_CLICKED(IDC_BTN_ENUMHERO, &CMainDlg::OnBnClickedBtnEnumhero)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &CMainDlg::OnEnChangeEdit1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_InitLOL2, &CMainDlg::OnBnClickedBtnInitlol2)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainDlg::OnBnClickedBtnInitlolclient()
{
	typedef void(WINAPI* InitHook_Pointer)();
	InitHook_Pointer InitHook;
	InitHook=(InitHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"StartHook_LOLClient");
	if(InitHook){
		InitHook();
	}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedBtnUninitlolclient()
{
	typedef void(WINAPI* UnHook_Pointer)();
	UnHook_Pointer UnHook;
	UnHook=(UnHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"UnHook_LOLClient");
	if(UnHook){
		UnHook();
	}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedBtnInitlol()
{
	BOOL bRET;
	typedef BOOL(WINAPI* InitHook_Pointer)();
	InitHook_Pointer InitHook;
	InitHook=(InitHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"ccvva");
	if(InitHook){
		bRET=InitHook();
		if(bRET){
			MessageBoxA("安装成功！","Notice",MB_OK);
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedBtnUninitlol()
{	
	
	typedef void(WINAPI* UnHook_Pointer)();
	UnHook_Pointer UnHook;
	UnHook=(UnHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"UnHook_LOL");
	if(UnHook){
	UnHook();
}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedBtnEnumhero()
{
	typedef void(WINAPI* SendCode_Pointer)(int Code);
	SendCode_Pointer SendCode;
	SendCode=(SendCode_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"SendCode");
	if(SendCode){
		SendCode(0x804);
	}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedButton2()
{
	typedef void(WINAPI* SendCode_Pointer)(int Code);
	SendCode_Pointer SendCode;
	SendCode=(SendCode_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"SendCode");
	if(SendCode){
		SendCode(0x805);
	}
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnBnClickedButton3()
{
	typedef void(WINAPI* ReloadConfig_Pointer)(LPCSTR lpPath);
	ReloadConfig_Pointer ReloadConfig;
	ReloadConfig=(ReloadConfig_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"LoadConfig");
	if(ReloadConfig){		
		char tmp[255];
		Edit_PATH.GetWindowText(tmp,255);
		ReloadConfig(tmp);
	}


	// TODO: 在此添加控件通知处理程序代码
}


void CMainDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CMainDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//取得被拖动文件的数目  
	for(int i=0;i< DropCount;i++)  
	{  
		CHAR wcStr[MAX_PATH];  
		DragQueryFile(hDropInfo,i,wcStr,MAX_PATH);//获得拖曳的第i个文件的文件名  
		Edit_PATH.SetWindowTextA(wcStr);
	}   
	DragFinish(hDropInfo);  //拖放结束后,释放内存  

	CDialogEx::OnDropFiles(hDropInfo);
}



void CMainDlg::OnBnClickedBtnInitlol2()
{
	BOOL bRET;
	typedef BOOL(WINAPI* InitHook_Pointer)();
	InitHook_Pointer InitHook;
	InitHook=(InitHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"StartHook_LOL2");
	if(InitHook){
		bRET=InitHook();
		if(bRET){
			MessageBoxA("安装成功！","Notice",MB_OK);
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}
