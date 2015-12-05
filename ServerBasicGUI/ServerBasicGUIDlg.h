
// ServerBasicGUIDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CServerBasicGUIDlg dialog
class CServerBasicGUIDlg : public CDialogEx
{
// Construction
public:
	CServerBasicGUIDlg(CWnd* pParent = NULL);	// standard constructor
private:
	bool running = false;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERBASICGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRunServer();
	afx_msg void OnDestroy();
	CEdit portNumber;
	CStatic simName;
	CButton runServer;
//	afx_msg void OnUpdatePortnumber();
//	afx_msg void OnChangePortnumber();
	afx_msg void OnChangePortnumber();
};
