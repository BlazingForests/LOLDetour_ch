
// MainDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMainDlg 对话框
class CMainDlg : public CDialogEx
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInitlolclient();
	afx_msg void OnBnClickedBtnUninitlolclient();
	afx_msg void OnBnClickedBtnInitlol();
	afx_msg void OnBnClickedBtnUninitlol();
	afx_msg void OnBnClickedBtnEnumhero();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit Edit_PATH;
	char EDIT_PATH_TEXT;
	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnInitlol2();
};
