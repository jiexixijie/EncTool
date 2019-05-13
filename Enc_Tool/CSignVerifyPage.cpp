// CSignVerifyPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CSignVerifyPage.h"
#include "afxdialogex.h"
#include "C_RSA.h"

int HashRSA_PaddingList[] = { RSA_PKCS1_PADDING, RSA_SSLV23_PADDING, RSA_NO_PADDING, RSA_PKCS1_OAEP_PADDING, RSA_X931_PADDING };
int HashTypeList[] = { Type_MD5, Type_SHA1, Type_SHA256 };
int Hash_BitsList[] = { 512,1024,2048 };
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
	DDX_Control(pDX, IDC_COMBO_KeyLen, m_keylen_cbox);
	DDX_Control(pDX, IDC_COMBO_CertType, m_CertType_cbox);
	DDX_Control(pDX, IDC_EDIT_encpwd, m_encpwd);
	DDX_Control(pDX, IDC_EDIT_Prikey, m_Privatekey);
	DDX_Control(pDX, IDC_EDIT_Pubkey, m_Publickey);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
	DDX_Control(pDX, IDC_COMBO_PaddingType, m_padding_cbox);
	DDX_Control(pDX, IDC_COMBO_HashType, m_HashType_cbox);
	DDX_Control(pDX, IDC_EncpwdStext, m_EncpwdStext);
}


BEGIN_MESSAGE_MAP(CSignVerifyPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CSignVerifyPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSignVerifyPage::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSignVerifyPage::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CSignVerifyPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GenKey, &CSignVerifyPage::OnBnClickedButtonGenkey)
	ON_BN_CLICKED(IDC_BUTTON2, &CSignVerifyPage::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO_CertType, &CSignVerifyPage::OnCbnSelchangeComboCerttype)
END_MESSAGE_MAP()


// CSignVerifyPage 消息处理程序


void CSignVerifyPage::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL isOpen = TRUE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		m_Privatekey.showFileData(OpenFileDlg.GetPathName());
		//直接从文件总读取到CString中显示
	}
	UpdateData(FALSE);
}


void CSignVerifyPage::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BOOL isOpen = TRUE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		m_Publickey.showFileData(OpenFileDlg.GetPathName());
		//直接从文件总读取到CString中显示
	}
	UpdateData(FALSE);
}


void CSignVerifyPage::OnBnClickedButton5()
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


void CSignVerifyPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//签名 读取KEY 输出签名字符串  Pri
	UpdateData(TRUE);
	CString Prikey, data;
	m_Privatekey.GetWindowTextW(Prikey);
	m_Data.GetWindowTextW(data);
	if (Prikey.IsEmpty()) {
		MessageBox(_T("私钥不能为空"));
		return;
	}
	if (data.IsEmpty()) {
		MessageBox(_T("请输入待加密明文"));
		return;
	}
	int index = m_keylen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	CString Prikey_Temp = TransData(Prikey, 1);
	C_RSA r;
	CString encpwd;
	m_encpwd.GetWindowTextW(encpwd);
	int padding = HashRSA_PaddingList[m_padding_cbox.GetCurSel()];
	if (r.Cstring_to_PriKey(Prikey_Temp, encpwd, m_CertType_cbox.GetCurSel()) != Success) {
		MessageBox(_T("读取私钥失败"));
		return;
	}
	//字符串加密
	int Hashtype = HashTypeList[m_HashType_cbox.GetCurSel()];
	if (!m_Data.IsFile()) {
		//私钥加密格式
		CString SignData;
		if (r.Pri_Sign(data, SignData, Hashtype) != Success) {
			MessageBox(_T("签名失败"));
			return;
		}
		m_EncData.SetWindowTextW(SignData);
	}
	//文件加密
	else {
		CFile file;
		if (file.Open(m_Data.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			this->SetWindowTextW(_T(""));
			return;
		}
		int filesize = (int)file.GetLength();
		char* filedata = new char[filesize+1];
		filedata[filesize] = '\0';
		if (filedata == NULL) {
			MessageBox(_T("申请内存失败"));
			return;
		}
		file.Read(filedata, filesize);
		char SignData[500];
		char SignDataBase[500];
		int signsize = 0;
		memset(SignData, 0, 500);
		memset(SignDataBase, 0, 500);
		r.Pri_Sign(filedata, filesize, SignData, signsize, Hashtype);
		EncBase64(SignData, signsize, SignDataBase, signsize);
		m_EncData.SetWindowTextW((CString)SignDataBase);
		delete[]filedata;
	}
	UpdateData(FALSE);
}


BOOL CSignVerifyPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_padding_cbox.SetCurSel(0);
	m_HashType_cbox.SetCurSel(1);
	m_keylen_cbox.SetCurSel(1);
	m_CertType_cbox.SetCurSel(0);
	m_Data.SetWindowTextW(_T("Welcome"));
	UpdateData(FALSE);
	OnBnClickedButtonGenkey();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSignVerifyPage::OnBnClickedButtonGenkey()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = 0, start = 0;
	CString temp;
	UpdateData(TRUE);
	C_RSA rsa;
	//0:512bit 1:1024bit 2:2048
	int bits = Hash_BitsList[m_keylen_cbox.GetCurSel()];
	int e_value = 0x10001;
	if (rsa.GenerateKey(bits, e_value) < 0) {
		MessageBox(_T("Generate Error"));
		return;
	}
	CString encpwd;
	m_encpwd.GetWindowTextW(encpwd);
	CString PriKey, PubKey;
	rsa.PriKey_to_Cstring(PriKey, encpwd, m_CertType_cbox.GetCurSel());
	rsa.PubKey_to_Cstring(PubKey, m_CertType_cbox.GetCurSel());
	//换行符转换 0x0A转0x0A0x0D
	PriKey = TransData(PriKey, 0);
	PubKey = TransData(PubKey, 0);
	m_Privatekey.SetWindowTextW(PriKey);
	m_Publickey.SetWindowTextW(PubKey);
	m_EncData.SetWindowTextW(_T(""));
	UpdateData(FALSE);
	return;
}


void CSignVerifyPage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//验证
	UpdateData(TRUE);
	CString Pubkey, data,encdata;
	m_Publickey.GetWindowTextW(Pubkey);
	m_Data.GetWindowTextW(data);
	m_EncData.GetWindowTextW(encdata);
	if (Pubkey.IsEmpty()) {
		MessageBox(_T("公钥不能为空"));
		return;
	}
	if (encdata.IsEmpty()) {
		MessageBox(_T("待验证数据"));
		return;
	}
	if (data.IsEmpty()) {
		MessageBox(_T("请输入原文件"));
		return;
	}
	int index = m_keylen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	CString Pubkey_Temp = TransData(Pubkey, 1);
	C_RSA r;
	int padding = HashRSA_PaddingList[m_padding_cbox.GetCurSel()];
	if (r.Cstring_to_PubKey(Pubkey_Temp, m_CertType_cbox.GetCurSel()) != Success) {
		MessageBox(_T("读取私钥失败"));
		return;
	}
	//字符串加密
	int Hashtype = HashTypeList[m_HashType_cbox.GetCurSel()];
	//获取签名值
	if (!m_Data.IsFile()) {
		//私钥加密格式
		TransData(data, 1);
		TransData(encdata, 1);
		//首先要将签名数据Base64还原
		char cipher_Base[500];
		memset(cipher_Base, 0, 500);
		int cipherlen = WideCharToMultiByte(CP_ACP, 0, encdata, encdata.GetLength(), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, encdata, encdata.GetLength(), (char*)cipher_Base, cipherlen, NULL, NULL);
		char cipher_c[500];
		DecBase64(cipher_Base, cipherlen, cipher_c, cipherlen);
		int datalen = WideCharToMultiByte(CP_ACP, 0, data, data.GetLength(), NULL, 0, NULL, NULL);
		char* data_c = new char[datalen + 1];
		memset(data_c, 0, datalen + 1);
		WideCharToMultiByte(CP_ACP, 0, data, data.GetLength(), (char*)data_c, datalen, NULL, NULL);
		int result = r.Pub_Verfiy(data_c, datalen, cipher_c, cipherlen, Hashtype);
		if (result == Success) {
			MessageBox(_T("验证正确"));
			delete[]data_c;
			return;
		}
		else if(result== VerifyFail){
			MessageBox(_T("验证失败"));
			delete[]data_c;
			return;
		}
		else {
			MessageBox(_T("出错了"));
			delete[]data_c;
			return;
		}
		delete[]data_c;
	}
	//文件加密
	else {
		CFile file;
		if (file.Open(m_Data.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			this->SetWindowTextW(_T(""));
			return;
		}
		int filesize = (int)file.GetLength();
		char* filedata = new char[filesize + 1];
		filedata[filesize] = '\0';
		if (filedata == NULL) {
			MessageBox(_T("申请内存失败"));
			return;
		}
		file.Read(filedata, filesize);
		file.Close();
		if (!m_EncData.IsFile()) {
			TransData(encdata, 1);
			//首先要将签名数据Base64还原
			char cipher_Base[500];
			memset(cipher_Base, 0, 500);
			int cipherlen = WideCharToMultiByte(CP_ACP, 0, encdata, encdata.GetLength(), NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, encdata, encdata.GetLength(), (char*)cipher_Base, cipherlen, NULL, NULL);
			char cipher_c[500];
			DecBase64(cipher_Base, cipherlen, cipher_c, cipherlen);
			int result = r.Pub_Verfiy(filedata, filesize, cipher_c, cipherlen, Hashtype);
			if (result == Success) {
				MessageBox(_T("验证正确"));
				delete[]filedata;
				return;
			}
			else if (result == VerifyFail) {
				MessageBox(_T("验证失败"));
				delete[]filedata;
				return;
			}
			else {
				MessageBox(_T("出错了"));
				delete[]filedata;
				return;
			}
			delete[]filedata;
		}
		//都是文件二进制数据比较
		else {
			if (file.Open(m_EncData.FilePath, CFile::modeRead) == 0) {
				MessageBox(_T("读取失败"));
				this->SetWindowTextW(_T(""));
				return;
			}
			int cipherlen = (int)file.GetLength();
			char* cipher_c = new char[cipherlen];
			cipher_c[cipherlen] = '\0';
			if (cipher_c == NULL) {
				MessageBox(_T("申请内存失败"));
				return;
			}
			file.Read(cipher_c, cipherlen);
			file.Close();
			int result = r.Pub_Verfiy(filedata, filesize, cipher_c, cipherlen, Hashtype);
			if (result == Success) {
				MessageBox(_T("验证正确"));
				delete[]filedata;
				delete[]cipher_c;
				return;
			}
			else if (result == VerifyFail) {
				MessageBox(_T("验证失败"));
				delete[]filedata;
				delete[]cipher_c;
				return;
			}
			else {
				MessageBox(_T("出错了"));
				delete[]filedata;
				delete[]cipher_c;
				return;
			}
			delete[]filedata;
			delete[]cipher_c;

		}
	}
	UpdateData(FALSE);
}


void CSignVerifyPage::OnCbnSelchangeComboCerttype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index = m_CertType_cbox.GetCurSel();
	m_Privatekey.SetWindowTextW(_T(""));
	m_Publickey.SetWindowTextW(_T(""));
	//PKCS#8 隐藏私钥密码两个控件并清空
	if (index == 0) {
		m_EncpwdStext.ShowWindow(SW_HIDE);
		m_encpwd.ShowWindow(SW_HIDE);
	}
	//PKCS#1 显示控件
	else {
		m_EncpwdStext.ShowWindow(SW_SHOW);
		m_encpwd.ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
}
