
// MainDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMainDlg �Ի���
class CMainDlg : public CDialogEx
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
