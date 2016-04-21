
// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
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


// CMainDlg �Ի���




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


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMainDlg::OnBnClickedBtnUninitlolclient()
{
	typedef void(WINAPI* UnHook_Pointer)();
	UnHook_Pointer UnHook;
	UnHook=(UnHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"UnHook_LOLClient");
	if(UnHook){
		UnHook();
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBoxA("��װ�ɹ���","Notice",MB_OK);
		}
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMainDlg::OnBnClickedBtnUninitlol()
{	
	
	typedef void(WINAPI* UnHook_Pointer)();
	UnHook_Pointer UnHook;
	UnHook=(UnHook_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"UnHook_LOL");
	if(UnHook){
	UnHook();
}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMainDlg::OnBnClickedBtnEnumhero()
{
	typedef void(WINAPI* SendCode_Pointer)(int Code);
	SendCode_Pointer SendCode;
	SendCode=(SendCode_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"SendCode");
	if(SendCode){
		SendCode(0x804);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMainDlg::OnBnClickedButton2()
{
	typedef void(WINAPI* SendCode_Pointer)(int Code);
	SendCode_Pointer SendCode;
	SendCode=(SendCode_Pointer)GetProcAddress(LoadLibrary("LOLDetour.dll"),"SendCode");
	if(SendCode){
		SendCode(0x805);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMainDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CMainDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//ȡ�ñ��϶��ļ�����Ŀ  
	for(int i=0;i< DropCount;i++)  
	{  
		CHAR wcStr[MAX_PATH];  
		DragQueryFile(hDropInfo,i,wcStr,MAX_PATH);//�����ҷ�ĵ�i���ļ����ļ���  
		Edit_PATH.SetWindowTextA(wcStr);
	}   
	DragFinish(hDropInfo);  //�ϷŽ�����,�ͷ��ڴ�  

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
			MessageBoxA("��װ�ɹ���","Notice",MB_OK);
		}
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
