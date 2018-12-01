
// GeneralInjectorDlg.h : header file
//

#pragma once

#include "InjectTool.h"
#include "CProcessListDlg.h"

// CGeneralInjectorDlg dialog
class CGeneralInjectorDlg : public CDialogEx
{
// Construction
public:
	CGeneralInjectorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENERALINJECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectProc();
	CComboBox m_CtrlInjectMethod;
	CEdit m_CtrlProcess;
	CMFCEditBrowseCtrl m_CtrlDllPath;
	//CProcessListDlg m_processListDlg;
	CString m_SelectedProcess;
	CString m_SelectedThread;
	HWND m_SelectedWnd;


	InjectTool m_InjectTool;
	afx_msg void OnBnClickedButtonInject();
};
