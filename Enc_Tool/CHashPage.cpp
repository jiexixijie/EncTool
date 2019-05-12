// CHashPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CHashPage.h"
#include "afxdialogex.h"
#include "C_Hash.h"

// CHashPage 对话框

IMPLEMENT_DYNAMIC(CHashPage, CDialogEx)

CHashPage::CHashPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_Hash, pParent)
	, m_MD5Data(_T(""))
	, m_SHA1Data(_T(""))
	, m_SHA256Data(_T(""))
{

}

CHashPage::~CHashPage()
{
}

void CHashPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT5, m_Data);
	DDX_Text(pDX, IDC_EDIT6, m_MD5Data);
	DDX_Text(pDX, IDC_EDIT7, m_SHA1Data);
	DDX_Text(pDX, IDC_EDIT8, m_SHA256Data);
}


BEGIN_MESSAGE_MAP(CHashPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CHashPage::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CHashPage::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHashPage 消息处理程序


void CHashPage::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL isOpen = TRUE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		//记录文件名
		m_Data.showFileHexData(OpenFileDlg.GetPathName());
	}
	UpdateData(FALSE);
}


void CHashPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString Data;
	m_Data.GetWindowTextW(Data);
	if (Data.IsEmpty()) {
		MessageBox(_T("数据不能为空"));
		return;
	}

	int index = 0;
	C_Hash h;
	if (!m_Data.IsFile()) {
		Data = TransData(Data, 1);
		if (h.Hash(Data, m_MD5Data, Type_MD5) != Success) {
			MessageBox(_T("MD5失败"));
			return;
		}
		if (h.Hash(Data, m_SHA1Data, Type_SHA1) != Success) {
			MessageBox(_T("SHA1失败"));
			return;
		}
		if (h.Hash(Data, m_SHA256Data, Type_SHA256) != Success) {
			MessageBox(_T("SHA256失败"));
			return;
		}
	}
	else {
		CFile fileR;
		if (fileR.Open(m_Data.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			m_MD5Data.Empty();
			m_SHA1Data.Empty();
			m_SHA256Data.Empty();
			return;
		}
		ULONGLONG filesize = fileR.GetLength();
		char* filedata = new char[filesize];
		char Cipher[256];
		int ciphersize = 0;
		if (filedata == NULL) {
			MessageBox(_T("申请内存失败"));
			fileR.Close();
			return;
		}
		fileR.Read(filedata, filesize);
		fileR.Close();
		//MD5
		if (h.Hash(filedata, filesize, Cipher, ciphersize, Type_MD5) != Success) {
			MessageBox(_T("MD5失败"));
			delete[]filedata;
			return;
		}
		else {
			m_MD5Data = Data_To_HexCString((BYTE*)Cipher, ciphersize);
		}
		//SHA1
		if (h.Hash(filedata, filesize, Cipher, ciphersize, Type_SHA1) != Success) {
			MessageBox(_T("SHA1失败"));
			delete[]filedata;
			return;
		}
		else {
			m_SHA1Data = Data_To_HexCString((BYTE*)Cipher, ciphersize);
		}
		//SHA256
		if (h.Hash(filedata, filesize, Cipher, ciphersize, Type_SHA256) != Success) {
			MessageBox(_T("SHA256失败"));
			delete[]filedata;
			return;
		}
		else {
			m_SHA256Data = Data_To_HexCString((BYTE*)Cipher, ciphersize);
		}
	}
	UpdateData(FALSE);
}
