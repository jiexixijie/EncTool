// CRC4Page.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CRC4Page.h"
#include "afxdialogex.h"
#include "C_RC4.h"

// CRC4Page 对话框

IMPLEMENT_DYNAMIC(CRC4Page, CDialogEx)

CRC4Page::CRC4Page(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_RC4, pParent)
	, m_key(_T(""))
{

}

CRC4Page::~CRC4Page()
{
}

void CRC4Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Key, m_key);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
}


BEGIN_MESSAGE_MAP(CRC4Page, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Enc, &CRC4Page::OnBnClickedButtonEnc)
	ON_BN_CLICKED(IDC_BUTTON_Dec, &CRC4Page::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_ImportEncData, &CRC4Page::OnBnClickedButtonImportencdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportData, &CRC4Page::OnBnClickedButtonImportdata)
END_MESSAGE_MAP()


// CRC4Page 消息处理程序


void CRC4Page::OnBnClickedButtonEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密
	UpdateData(TRUE);
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	if (m_key.GetLength() < 5 || m_key.GetLength() > 256) {
		CString msg;
		msg.Format(_T("密钥长度请在5Bytes-256Bytes"));
		MessageBox(msg);
		return;
	}
	//判断明文不为空
	CString Data;
	m_Data.GetWindowTextW(Data);
	if (Data.IsEmpty()) {
		MessageBox(_T("请输入待加密数据"));
		return;
	}
	//将key为char 同时如果不足 用'\0'末尾进行补充
	char key_c[256];
	memset(key_c, 0, 256);
	int keylen = WideCharToMultiByte(CP_ACP, 0, m_key, m_key.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, m_key, m_key.GetLength(), key_c, keylen, NULL, NULL);
	C_RC4 rc;
	//字符串加密 BASE64显示
	if (!m_Data.IsFile()) {
		CString EncData;
		//windows换行符为'/r/n' 转换为'/n'
		Data = TransData(Data, 1);
		if (rc.EncData(Data, EncData, key_c, keylen) != Success) {
			MessageBox(_T("加密失败"));
		}
		else {
			m_EncData.SetWindowTextW(EncData);
		}
	}
	//文件加解密
	else {
		CFile fileR, fileW;
		CString FileW_path = FileDlg_GetSavePath();
		if (m_Data.FilePath == FileW_path) {
			MessageBox(_T("两个文件应不同"));
			return;
		}
		if (fileR.Open(m_Data.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			m_EncData.SetWindowTextW(_T(""));
			return;
		}
		if (fileW.Open(FileW_path, CFile::modeCreate | CFile::modeReadWrite) == 0) {
			MessageBox(_T("创建失败"));
			m_EncData.SetWindowTextW(_T(""));
			return;
		}
		ULONGLONG filesize = fileR.GetLength();
		char filedata[1024], CipherData[1024];
		int Readsize = 1024;
		int cipherlen = 0;
		//每1024字节一个循环 加密文件
		while (Readsize < filesize) {
			fileR.Read(filedata, 1024);
			if (rc.EncData(filedata, 1024, CipherData, cipherlen, key_c, keylen) != Success) {
				MessageBox(_T("加密失败"));
				m_EncData.SetWindowTextW(_T(""));
				fileR.Close();
				fileW.Close();
				return;
			}
			//写入部分密文
			fileW.Write(CipherData, 1024);
			filesize -= 1024;
		}
		if (filesize != 0) {
			memset(filedata, 0, sizeof(char) * 1024);
			memset(CipherData, 0, sizeof(char) * 1024);
			fileR.Read(filedata, filesize);
			if (rc.EncData(filedata, filesize, CipherData, cipherlen, key_c, keylen) != Success) {
				MessageBox(_T("加密失败"));
				m_EncData.SetWindowTextW(_T(""));
				fileR.Close();
				fileW.Close();
				return;
			}
			fileW.Write(CipherData, cipherlen);
		}
		fileR.Close();
		fileW.Close();
		//显示加密后密文
		m_EncData.showFileHexData(FileW_path);
	}
	UpdateData(FALSE);
}


void CRC4Page::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	if (m_key.GetLength() < 5 || m_key.GetLength() > 256) {
		CString msg;
		msg.Format(_T("密钥长度请在5Bytes-256Bytes"));
		MessageBox(msg);
		return;
	}
	//判断密文不为空
	CString EncData;
	m_EncData.GetWindowTextW(EncData);
	if (EncData.IsEmpty()) {
		MessageBox(_T("请输入待加密数据"));
		return;
	}
	//将key为char 同时如果不足 用'\0'末尾进行补充
	char key_c[256];
	memset(key_c, 0, 256);
	int keylen = WideCharToMultiByte(CP_ACP, 0, m_key, m_key.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, m_key, m_key.GetLength(), key_c, keylen, NULL, NULL);
	C_RC4 rc;
	//字符串解密 BASE64显示
	if (!m_EncData.IsFile()) {
		CString Data;
		if (rc.DecData(EncData, Data, key_c, keylen) != Success) {
			MessageBox(_T("解密失败"));
		}
		Data = TransData(Data, 0);
		m_Data.SetWindowTextW(Data);
	}
	else {
		CFile fileR, fileW;
		CString FileW_path = FileDlg_GetSavePath();
		if (m_EncData.FilePath == FileW_path) {
			MessageBox(_T("两个文件应不同"));
			return;
		}
		if (fileR.Open(m_EncData.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			m_Data.SetWindowTextW(_T(""));
			return;
		}
		if (fileW.Open(FileW_path, CFile::modeCreate | CFile::modeReadWrite) == 0) {
			MessageBox(_T("创建失败"));
			m_Data.SetWindowTextW(_T(""));
			return;
		}
		ULONGLONG filesize = fileR.GetLength();
		char filedata[1024], DecData[1024];
		int Readsize = 1024;
		int DecDatalen = 0;
		//每1024字节一个循环 解密文件
		while (Readsize < filesize) {
			fileR.Read(filedata, 1024);
			if (rc.DecData(filedata, 1024, DecData, DecDatalen, key_c,keylen) != Success) {
				MessageBox(_T("解密失败"));
				m_Data.SetWindowTextW(_T(""));
				fileR.Close();
				fileW.Close();
				return;
			}
			//写入部分密文
			fileW.Write(DecData, 1024);
			filesize -= 1024;
		}
		if (filesize != 0) {
			memset(filedata, 0, sizeof(char) * 1024);
			memset(DecData, 0, sizeof(char) * 1024);
			fileR.Read(filedata, filesize);
			if (rc.DecData(filedata, filesize, DecData, DecDatalen, key_c, keylen) != Success) {
				MessageBox(_T("解密失败"));
				m_Data.SetWindowTextW(_T(""));
				fileR.Close();
				fileW.Close();
				return;
			}
			fileW.Write(DecData, DecDatalen);
		}
		fileR.Close();
		fileW.Close();
		//显示解密后数据
		m_Data.showFileData(FileW_path);
	}
	UpdateData(FALSE);
}


BOOL CRC4Page::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Data.SetWindowTextW(_T("Welcome"));
	m_key.Format(_T("12345"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRC4Page::OnBnClickedButtonImportencdata()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL isOpen = TRUE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		//记录文件名
		m_EncData.showFileHexData(OpenFileDlg.GetPathName());
	}
	UpdateData(FALSE);
}


void CRC4Page::OnBnClickedButtonImportdata()
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
