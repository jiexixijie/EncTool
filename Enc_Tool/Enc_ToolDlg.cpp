
// Enc_ToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Enc_Tool.h"
#include "Enc_ToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEncToolDlg 对话框



CEncToolDlg::CEncToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENC_TOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_CurSelTab = 0;
	for (int i = 0; i < 10; i++) {
		pages[i] = NULL;
	}
}

void CEncToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CEncToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEncToolDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CEncToolDlg 消息处理程序

BOOL CEncToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	int PageNums = 0;
	//设置页面标签
	m_tab.InsertItem(0, _T("RSA"));
	m_tab.InsertItem(1, _T("DES"));
	m_tab.InsertItem(2, _T("3DES"));
	m_tab.InsertItem(3, _T("AES"));
	m_tab.InsertItem(4, _T("Blowfish"));
	m_tab.InsertItem(5, _T("RC4"));
	m_tab.InsertItem(6, _T("Hash"));
	m_tab.InsertItem(7, _T("Sign/Verify"));
	m_tab.InsertItem(8, _T("Other"));


	RSApage.Create(DIALOG_RSA, &m_tab); pages[PageNums++] = &RSApage;
	DESpage.Create(DIALOG_DES, &m_tab); pages[PageNums++] = &DESpage;
	DES3page.Create(DIALOG_3DES, &m_tab); pages[PageNums++] = &DES3page;
	AESpage.Create(DIALOG_AES, &m_tab); pages[PageNums++] = &AESpage;
	Blowfishpage.Create(DIALOG_Blowfish, &m_tab); pages[PageNums++] = &Blowfishpage;
	RC4page.Create(DIALOG_RC4, &m_tab); pages[PageNums++] = &RC4page;
	Hashpage.Create(DIALOG_Hash, &m_tab); pages[PageNums++] = &Hashpage;
	SignVerifypage.Create(DIALOG_SignVerify, &m_tab); pages[PageNums++] = &SignVerifypage;
	Otherpage.Create(DIALOG_Other, &m_tab); pages[PageNums++] = &Otherpage;

	CRect rc;
	m_tab.GetClientRect(rc); //获得m_tab对话框尺寸
	rc.top += 26;
	//设置举行对话框尺寸参数  默认隐藏
	for (int i = 0; i < PageNums; i++) {
		pages[i]->MoveWindow(&rc);
		pages[i]->ShowWindow(SW_HIDE);
	}

	m_tab.SetCurSel(0);
	m_CurSelTab = 0;
	//移动对话框
	pages[m_CurSelTab]->ShowWindow(SW_SHOW);
	//默认选项为RSA界面


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEncToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEncToolDlg::OnPaint()
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
HCURSOR CEncToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CEncToolDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//切换页面  隐藏之前的页面
	pages[m_CurSelTab]->ShowWindow(SW_HIDE);
	//更新m_CurSelTab
	m_CurSelTab = m_tab.GetCurSel();
	pages[m_CurSelTab]->ShowWindow(SW_SHOW);

}
