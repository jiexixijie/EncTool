// CBlowfishPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CBlowfishPage.h"
#include "afxdialogex.h"


// CBlowfishPage 对话框

IMPLEMENT_DYNAMIC(CBlowfishPage, CDialogEx)

CBlowfishPage::CBlowfishPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_Blowfish, pParent)
{

}

CBlowfishPage::~CBlowfishPage()
{
}

void CBlowfishPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_enctype_cbox);
	DDX_Control(pDX, IDC_COMBO3, m_padding_cbox);
}


BEGIN_MESSAGE_MAP(CBlowfishPage, CDialogEx)
END_MESSAGE_MAP()


// CBlowfishPage 消息处理程序


BOOL CBlowfishPage::OnInitDialog()
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
