
// ServerBasicGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerBasicGUI.h"
#include "ServerBasicGUIDlg.h"
#include "afxdialogex.h"

#include "../ServerCore/WebServer.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_LOADSTRING 100

char address[MAX_LOADSTRING] = "8080";

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

void ConnectionInfoUpdater(void * target)
{
	CEdit * connDisplay = (CEdit*)target;

	for (;;)
	{
		if (webserverSimName() != NULL)
		{
			connDisplay->SetWindowText(webserverSimName()->c_str());
		}
		else
		{
			CString simNameString;
			simNameString.LoadStringW(IDS_NOT_CONNECTED);
			connDisplay->SetWindowText(simNameString);
		}
		Sleep(1000);
	}
}

CServerBasicGUIDlg::CServerBasicGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVERBASICGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerBasicGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORTNUMBER, portNumber);
	DDX_Control(pDX, IDC_SIMNAME, simName);
	DDX_Control(pDX, IDC_RUNSERVER, runServer);
}

BEGIN_MESSAGE_MAP(CServerBasicGUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUNSERVER, &CServerBasicGUIDlg::OnBnClickedRunServer)
	ON_WM_DESTROY()
ON_EN_CHANGE(IDC_PORTNUMBER, &CServerBasicGUIDlg::OnChangePortnumber)
END_MESSAGE_MAP()


// CServerBasicGUIDlg message handlers

BOOL CServerBasicGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_beginthread(ConnectionInfoUpdater, 0, &simName);
	CString simNameString;
	simNameString.LoadStringW(IDS_NOT_CONNECTED);
	simName.SetWindowText(simNameString);

	CString portNumberString("8080");
	portNumber.SetWindowText(portNumberString);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerBasicGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerBasicGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerBasicGUIDlg::OnBnClickedRunServer()
{
	if (runServer.GetCheck())
	{
		portNumber.SetReadOnly(true);
		launchServer(address, "dashboards");
	}
	else
	{
		stopServer();
		portNumber.SetReadOnly(false);
		CString simNameString;
		simNameString.LoadStringW(IDS_NOT_CONNECTED);
		simName.SetWindowText(simNameString);
	}

	if (webserverSimName() != NULL)
	{
		simName.SetWindowText(webserverSimName()->c_str());
	}

}


void CServerBasicGUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	stopServer();
}

void CServerBasicGUIDlg::OnChangePortnumber()
{
	WCHAR addrMem[MAX_LOADSTRING];
	portNumber.GetWindowText(addrMem, MAX_LOADSTRING);
	for (int i = 0; i < 7; i++)
	{
		address[i] = addrMem[i];
	}
}


