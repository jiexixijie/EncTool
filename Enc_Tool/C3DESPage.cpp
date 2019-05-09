// C3DESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "C3DESPage.h"
#include "afxdialogex.h"


// C3DESPage 对话框

IMPLEMENT_DYNAMIC(C3DESPage, CDialogEx)

C3DESPage::C3DESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_3DES, pParent)
{

}

C3DESPage::~C3DESPage()
{
}

void C3DESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_enctype_cbox);
	DDX_Control(pDX, IDC_COMBO3, m_padding_cbox);
}


BEGIN_MESSAGE_MAP(C3DESPage, CDialogEx)
END_MESSAGE_MAP()


// C3DESPage 消息处理程序


BOOL C3DESPage::OnInitDialog()
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
