// COtherPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "COtherPage.h"
#include "afxdialogex.h"


// COtherPage 对话框

IMPLEMENT_DYNAMIC(COtherPage, CDialogEx)

COtherPage::COtherPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_Other, pParent)
{

}

COtherPage::~COtherPage()
{
}

void COtherPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COtherPage, CDialogEx)
END_MESSAGE_MAP()


// COtherPage 消息处理程序
