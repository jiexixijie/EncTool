// CAESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CAESPage.h"
#include "afxdialogex.h"


// CAESPage 对话框

IMPLEMENT_DYNAMIC(CAESPage, CDialogEx)

CAESPage::CAESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_AES, pParent)
{

}

CAESPage::~CAESPage()
{
}

void CAESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_enctype_cbox);
	DDX_Control(pDX, IDC_COMBO3, m_paddingtype_cbox);
	DDX_Control(pDX, IDC_COMBO5, m_blocksize_cbox);
}


BEGIN_MESSAGE_MAP(CAESPage, CDialogEx)
END_MESSAGE_MAP()


// CAESPage 消息处理程序


BOOL CAESPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认为CBC模式
	m_enctype_cbox.SetCurSel(0);
	//填充模式
	m_paddingtype_cbox.SetCurSel(0);
	m_blocksize_cbox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
