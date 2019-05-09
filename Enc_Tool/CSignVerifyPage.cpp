// CSignVerifyPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CSignVerifyPage.h"
#include "afxdialogex.h"


// CSignVerifyPage 对话框

IMPLEMENT_DYNAMIC(CSignVerifyPage, CDialogEx)

CSignVerifyPage::CSignVerifyPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_SignVerify, pParent)
{

}

CSignVerifyPage::~CSignVerifyPage()
{
}

void CSignVerifyPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_hashtype_cbox);
}


BEGIN_MESSAGE_MAP(CSignVerifyPage, CDialogEx)
END_MESSAGE_MAP()


// CSignVerifyPage 消息处理程序


BOOL CSignVerifyPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_hashtype_cbox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
