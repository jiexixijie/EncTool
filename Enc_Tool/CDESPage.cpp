// CDESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CDESPage.h"
#include "afxdialogex.h"


// CDESPage 对话框

IMPLEMENT_DYNAMIC(CDESPage, CDialogEx)

CDESPage::CDESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_DES, pParent)
{

}

CDESPage::~CDESPage()
{
}

void CDESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_enctype_cbox);
	DDX_Control(pDX, IDC_COMBO3, m_padding_cbox);
}


BEGIN_MESSAGE_MAP(CDESPage, CDialogEx)
END_MESSAGE_MAP()


// CDESPage 消息处理程序


void CDESPage::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}


BOOL CDESPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认为CBC模式
	m_enctype_cbox.SetCurSel(0);
	//填充模式
	m_padding_cbox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
