// CAESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CAESPage.h"
#include "afxdialogex.h"
#include "C_AES.h"
#include <openssl/evp.h>

int AES_TYPE_List[] = { CBC,ECB, OFB, CFB };

// CAESPage 对话框

IMPLEMENT_DYNAMIC(CAESPage, CDialogEx)

CAESPage::CAESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_AES, pParent)
	, m_key(_T(""))
{

}

CAESPage::~CAESPage()
{
}

void CAESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT5, m_Data);
	DDX_Control(pDX, IDC_EDIT6, m_EncData);
	DDX_Control(pDX, IDC_COMBO3, m_paddding_cbox);
	DDX_Control(pDX, IDC_COMBO2, m_EncType_cbox);
	DDX_Text(pDX, IDC_EDIT1, m_key);
	DDX_Control(pDX, IDC_EDIT4, m_iv);
	DDX_Control(pDX, IDC_BUTTON1, m_ivText);
	DDX_Control(pDX, IDC_COMBO5, m_KeySize_cbox);
}


BEGIN_MESSAGE_MAP(CAESPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAESPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAESPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_ImportData, &CAESPage::OnBnClickedButtonImportdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportEncData, &CAESPage::OnBnClickedButtonImportencdata)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAESPage::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CAESPage 消息处理程序


void CAESPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//确认加密密码不为空
	UpdateData(TRUE);
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	//字节
	int KeySize[] = { 16,24,32 };
	if (m_key.GetLength() > KeySize[m_KeySize_cbox.GetCurSel()]) {
		CString msg;
		msg.Format(_T("密钥长度不能超过%dbits"), KeySize[m_KeySize_cbox.GetCurSel()] * 8);
		MessageBox(msg);
		return;
	}
	//检查iv 不超过16字节
	CString iv;
	m_iv.GetWindowTextW(iv);
	if (iv.GetLength() > 16) {
		MessageBox(_T("iv长度不能超过16Bytes"));
		return;
	}
	//判断明文不为空
	CString Data;
	m_Data.GetWindowTextW(Data);
	if (Data.IsEmpty()) {
		MessageBox(_T("请输入待加密数据"));
		return;
	}
	//将key和iv转化为char 同时如果不足 用'\0'末尾进行补充
    //同时若iv为空则设置为NULL 32能存储最长的长度了
	char key_c[32], iv_c[16];
	memset(key_c, 0, 32);
	memset(iv_c, 0, 16);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_AES a;
	//设置大小 否则默认为128字节
	a.SetKeySize(KeySize[m_KeySize_cbox.GetCurSel()] * 8);
	//字符串加密 BASE64显示
	if (!m_Data.IsFile()) {
		CString EncData;
		//windows换行符为'/r/n' 转换为'/n'
		Data = TransData(Data, 1);
		if (a.EncData(Data, EncData, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
			if (a.EncData(filedata, 1024, CipherData, cipherlen, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
			if (a.EncData(filedata, filesize, CipherData, cipherlen, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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




void CAESPage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	//字节
	int KeySize[] = { 16,24,32 };
	if (m_key.GetLength() > KeySize[m_KeySize_cbox.GetCurSel()]) {
		CString msg;
		msg.Format(_T("密钥长度不能超过%dbits"), KeySize[m_KeySize_cbox.GetCurSel()] * 8);
		MessageBox(msg);
		return;
	}
	//检查iv 不超过16字节
	CString iv;
	m_iv.GetWindowTextW(iv);
	if (iv.GetLength() > 16) {
		MessageBox(_T("iv长度不能超过16Bytes"));
		return;
	}
	//判断密文不为空
	CString EncData;
	m_EncData.GetWindowTextW(EncData);
	if (EncData.IsEmpty()) {
		MessageBox(_T("请输入待解密密文"));
		return;
	}
	//将key和iv转化为char 同时如果不足8字符用'\0'末尾进行补充
	//同时若iv为空则设置为NULL 使用C_DES 默认iv:12345678
	char key_c[32], iv_c[16];
	memset(key_c, 0, 32);
	memset(iv_c, 0, 16);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_AES a;
	a.SetKeySize(KeySize[m_KeySize_cbox.GetCurSel()] * 8);
	//字符串解密 BASE64显示
	if (!m_EncData.IsFile()) {
		CString Data;
		if (a.DecData(EncData, Data, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
			if (a.DecData(filedata, 1024, DecData, DecDatalen, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
			if (a.DecData(filedata, filesize, DecData, DecDatalen, key_c, iv_c, AES_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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


BOOL CAESPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Data.SetWindowTextW(_T("Welcome"));
	m_paddding_cbox.SetCurSel(0);
	m_EncType_cbox.SetCurSel(0);
	m_KeySize_cbox.SetCurSel(0);
	m_iv.SetWindowTextW(_T("1234567812345678"));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAESPage::OnBnClickedButtonImportdata()
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


void CAESPage::OnBnClickedButtonImportencdata()
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


void CAESPage::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index = m_EncType_cbox.GetCurSel();
	if (index == 2 || index == 3) {
		m_iv.ShowWindow(SW_HIDE);
		m_ivText.ShowWindow(SW_HIDE);
	}
	else {
		m_iv.ShowWindow(SW_SHOW);
		m_ivText.ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
}
