// CRSAPagePri.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CRSAPagePri.h"
#include "afxdialogex.h"


// CRSAPagePri 对话框

IMPLEMENT_DYNAMIC(CRSAPagePri, CDialogEx)

CRSAPagePri::CRSAPagePri(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_RSA, pParent)
{

}

CRSAPagePri::~CRSAPagePri()
{
}

void CRSAPagePri::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRSAPagePri, CDialogEx)
END_MESSAGE_MAP()


// CRSAPagePri 消息处理程序
