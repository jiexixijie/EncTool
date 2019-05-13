// CRSAPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CRSAPage.h"
#include "afxdialogex.h"
#include "C_RSA.h"
#include <openssl/pem.h>

int RSA_PaddingList[] = { RSA_PKCS1_PADDING, RSA_SSLV23_PADDING, RSA_NO_PADDING, RSA_PKCS1_OAEP_PADDING, RSA_X931_PADDING };
int RSA_BitsList[] = { 512,1024,2048 };
// CRSAPage 对话框

IMPLEMENT_DYNAMIC(CRSAPage, CDialog)

CRSAPage::CRSAPage(CWnd* pParent /*=nullptr*/)
	: CDialog(RSA_DIALOG)
	, m_Evalue(_T(""))
{

}

CRSAPage::~CRSAPage()
{
}

void CRSAPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_HexDec_cbox);
	DDX_Control(pDX, IDC_COMBO_PaddingType, m_pading_cbox);
	DDX_Control(pDX, IDC_COMBO_KeyLen, m_ketlen_cbox);
	DDX_Text(pDX, IDC_EDIT_eValue, m_Evalue);
	DDX_Control(pDX, IDC_EDIT_Prikey, m_Privatekey);
	DDX_Control(pDX, IDC_EDIT_Pubkey, m_Publickey);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
	DDX_Control(pDX, IDC_EDIT_encpwd, m_encpwd);
	DDX_Control(pDX, IDC_COMBO_CertType, m_CertType);
	DDX_Control(pDX, IDC_EncpwdStext, m_encpwdStext);
}


BEGIN_MESSAGE_MAP(CRSAPage, CDialog)
ON_CBN_SELCHANGE(IDC_COMBO1, &CRSAPage::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_BUTTON11, &CRSAPage::OnBnClickedButtonGenkey)
ON_BN_CLICKED(IDC_BUTTON7, &CRSAPage::OnBnClickedButtonImportPri)
ON_BN_CLICKED(IDC_BUTTON1, &CRSAPage::OnBnClickedButtonPubEnc)
ON_BN_CLICKED(IDC_BUTTON12, &CRSAPage::OnBnClickedButtonPriEnc)
ON_BN_CLICKED(IDC_BUTTON2, &CRSAPage::OnBnClickedButtonPubDec)
ON_BN_CLICKED(IDC_BUTTON13, &CRSAPage::OnBnClickedButtonPriDec)
ON_CBN_SELCHANGE(IDC_COMBO_CertType, &CRSAPage::OnCbnSelchangeCombo6)
//ON_WM_DROPFILES()
ON_BN_CLICKED(IDC_BUTTON3, &CRSAPage::OnBnClickedButtonOpenPri)
ON_BN_CLICKED(IDC_BUTTON_GenKey, &CRSAPage::OnBnClickedButtonGenkey)
ON_BN_CLICKED(IDC_BUTTON4, &CRSAPage::OnBnClickedButtonOpenPub)
ON_BN_CLICKED(IDC_BUTTON8, &CRSAPage::OnBnClickedButtonImportPub)
ON_BN_CLICKED(IDC_BUTTON5, &CRSAPage::OnBnClickedButtonImportData)
ON_BN_CLICKED(IDC_BUTTON6, &CRSAPage::OnBnClickedButtonImportEncData)
ON_CBN_SELCHANGE(IDC_COMBO_KeyLen, &CRSAPage::OnCbnSelchangeComboKeylen)
END_MESSAGE_MAP()


// CRSAPage 消息处理程序


BOOL CRSAPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认设置为16进制
	m_HexDec_cbox.SetCurSel(0);
	//设置默认填充方式为RSA_PKCS1_PADDING
	m_pading_cbox.SetCurSel(0);
	//密钥长度为1024
	m_ketlen_cbox.SetCurSel(1);
	//设置指数E默认为0x10001
	m_Evalue = "0x10001";
	//设置默认密钥格式默认为PKCS#8
	m_CertType.SetCurSel(0);
	m_encpwd.ShowWindow(SW_HIDE);
	m_encpwdStext.ShowWindow(SW_HIDE);
	m_Data.SetWindowTextW(_T("Welcome"));
	UpdateData(FALSE);
	OnBnClickedButtonGenkey();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRSAPage::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CRSAPage::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	//改变框中内容
	UpdateData(TRUE);
	if (m_Evalue.IsEmpty()) {
		m_Evalue = _T("0");
	}
	int index = m_HexDec_cbox.GetCurSel();
	//10进制转16进制
	if (index == 0) {
		int tmp = _ttoi(m_Evalue);
		m_Evalue.Format(_T("0x%x"), tmp);
	}
	//16进制转10进制
	else {
		int tmp;
		StrToIntEx(m_Evalue, STIF_SUPPORT_HEX, &tmp);
		m_Evalue.Format(_T("%d"), tmp);
	}
	UpdateData(FALSE);
	
}



void CRSAPage::OnBnClickedButtonGenkey()
{ 
	// TODO: 在此添加控件通知处理程序代码
	//生成公私密钥对
	//PEM格式显示私钥和公钥
	int index = 0,start=0;
	CString temp;
	UpdateData(TRUE);
	if (m_Evalue.IsEmpty()) {
		MessageBox(_T("指数不能为空"));
	}
	C_RSA rsa;
	//0:512bit 1:1024bit 2:2048
	int bits = RSA_BitsList[m_ketlen_cbox.GetCurSel()];
	//CString16 进制转换
	int e_value= _tcstol(m_Evalue, NULL, 16);
	if (rsa.GenerateKey(bits, e_value) < 0) {
		MessageBox(_T("Generate Error"));
		return;
	}
	CString encpwd;
	m_encpwd.GetWindowTextW(encpwd);
	CString PriKey,PubKey;
	rsa.PriKey_to_Cstring(PriKey, encpwd, m_CertType.GetCurSel());
	rsa.PubKey_to_Cstring(PubKey, m_CertType.GetCurSel());
	//换行符转换 0x0A转0x0A0x0D
	PriKey = TransData(PriKey,0);
	PubKey = TransData(PubKey,0);
	m_Privatekey.SetWindowTextW(PriKey);
	m_Publickey.SetWindowTextW(PubKey);
	m_EncData.SetWindowTextW(_T(""));
	UpdateData(FALSE);
	return;
}


void CRSAPage::OnBnClickedButtonPubEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//公钥加密 判断公钥和明文是否存在
	//判断是加密文件还是加密text字符串
	UpdateData(TRUE);
	CString Pubkey, data;
	m_Publickey.GetWindowTextW(Pubkey);
	m_Data.GetWindowTextW(data);
	if (Pubkey.IsEmpty()) {
		MessageBox(_T("公钥不能为空"));
		return;
	}
	if (data.IsEmpty()) {
		MessageBox(_T("请输入待加密明文"));
		return;
	}
	int index = m_ketlen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	//读取公钥
	C_RSA r;
	int padding = RSA_PaddingList[m_pading_cbox.GetCurSel()];
	CString Pubkey_Temp = TransData(Pubkey, 1);
	if (r.Cstring_to_PubKey(Pubkey_Temp, m_CertType.GetCurSel()) != Success) {
		MessageBox(_T("读取公钥失败"));
		return;
	}
	//如果是加密字符串
	if (!m_Data.IsFile()) {
		CString Encdata;
		if (data.GetLength() > (bitlen[index] / 8 - 11)) {
			CString mess;
			mess.Format(_T("待加密明文过长,请输入低于%dByte明文"), bitlen[index] / 8 - 11);
			MessageBox(mess);
			return;
		}
		if (r.DealData(data, Encdata, RSA_Pub_Enc, padding) < 0) {
			MessageBox(_T("加密错误"));
			return;
		}
		//显示加密后密文
		m_EncData.SetWindowTextW(Encdata);
	}
	//加密文件 直接弹出对话框保存到文件中
	else {
		CFile file;
		if(file.Open(m_Data.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			this->SetWindowTextW(_T(""));
			return;
		}
		int filesize = (int)file.GetLength();
		if (filesize > (bitlen[index] / 8 - 11)) {
			MessageBox(_T("文件应小于加密密钥大小"));
			return;
		}
		char filedata[1000], DealData[1000];
		memset(filedata, 0, sizeof(char) * 1000);
		memset(DealData, 0, sizeof(char) * 1000);
		file.Read(filedata, filesize);
		file.Close();
		int DealDatalen = 0;
		if (r.DealData(filedata, filesize,DealData,DealDatalen, RSA_Pub_Enc, padding) < 0) {
			MessageBox(_T("加密错误"));
			return;
		}
		CString filepath= FileDlg_Save(DealData, DealDatalen);
		//显示加密后密文
		m_EncData.showFileHexData(filepath);

	}
	UpdateData(FALSE);

}


void CRSAPage::OnBnClickedButtonPriEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//私钥加密
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
	int index = m_ketlen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	CString Prikey_Temp = TransData(Prikey, 1);
	C_RSA r;
	CString encpwd;
	m_encpwd.GetWindowTextW(encpwd);
	int padding = RSA_PaddingList[m_pading_cbox.GetCurSel()];
	if (r.Cstring_to_PriKey(Prikey_Temp, encpwd, m_CertType.GetCurSel()) != Success) {
		MessageBox(_T("读取私钥失败"));
		return;
	}
	//字符串加密
	if (!m_Data.IsFile()) {
		//私钥加密格式
		if (data.GetLength() > (bitlen[index] / 8 - 11)) {
			CString mess;
			mess.Format(_T("待加密明文过长,请输入低于%dByte明文"), bitlen[index] / 8 - 11);
			MessageBox(mess);
			return;
		}
		CString Encdata;
		if (r.DealData(data, Encdata, RSA_Pri_Enc, padding) < 0) {
			MessageBox(_T("加密错误"));
			return;
		}
		m_EncData.SetWindowTextW(Encdata);
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
		if (filesize > (bitlen[index] / 8 - 11)) {
			MessageBox(_T("文件应小于加密密钥大小"));
			return;
		}
		char filedata[1000], DealData[1000];
		memset(filedata, 0, sizeof(char) * 1000);
		memset(DealData, 0, sizeof(char) * 1000);
		file.Read(filedata, filesize);
		file.Close();
		int DealDatalen = 0;
		if (r.DealData(filedata, filesize, DealData, DealDatalen, RSA_Pri_Enc, padding) < 0) {
			MessageBox(_T("加密错误"));
			return;
		}
		CString filepath = FileDlg_Save(DealData, DealDatalen);
		//显示加密后密文
		m_EncData.showFileHexData(filepath);
	}
	UpdateData(FALSE);
}


void CRSAPage::OnBnClickedButtonPubDec()
{
	// TODO: 在此添加控件通知处理程序代码
	//公钥解密
	UpdateData(TRUE);
	CString Pubkey, Encdata;
	m_Publickey.GetWindowTextW(Pubkey);
	m_EncData.GetWindowTextW(Encdata);
	if (Pubkey.IsEmpty()) {
		MessageBox(_T("公钥不能为空"));
		return;
	}
	if (Encdata.IsEmpty()) {
		MessageBox(_T("请输入待加密明文"));
		return;
	}
	int index = m_ketlen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	//读取公钥
	C_RSA r;
	int padding = RSA_PaddingList[m_pading_cbox.GetCurSel()];
	CString Pubkey_Temp = TransData(Pubkey, 1);
	if (r.Cstring_to_PubKey(Pubkey_Temp, m_CertType.GetCurSel()) != Success) {
		MessageBox(_T("读取公钥失败"));
		return;
	}
	//如果是解密字符串
	if (!m_EncData.IsFile()) {
		CString data;
		if (r.DealData(Encdata, data, RSA_Pub_Dec, padding) < 0) {
			MessageBox(_T("解密错误"));
			return;
		}
		//显示解密后数据
		m_Data.SetWindowTextW(data);
	}
	//解密文件 直接弹出对话框保存到文件中
	else {
		CFile file;
		if (file.Open(m_EncData.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			this->SetWindowTextW(_T(""));
			return;
		}
		int filesize = (int)file.GetLength();
		char filedata[1000], DealData[1000];
		memset(filedata, 0, sizeof(char) * 1000);
		memset(DealData, 0, sizeof(char) * 1000);
		file.Read(filedata, filesize);
		file.Close();
		int DealDatalen = 0;
		if (r.DealData(filedata, filesize, DealData, DealDatalen, RSA_Pub_Dec, padding) < 0) {
			MessageBox(_T("解密错误"));
			return;
		}
		//显示解密后数据
		CString filepath = FileDlg_Save(DealData, DealDatalen);
		m_Data.showFileData(filepath);
	}
	UpdateData(FALSE);
}


void CRSAPage::OnBnClickedButtonPriDec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString Prikey, Encdata;
	m_Privatekey.GetWindowTextW(Prikey);
	m_EncData.GetWindowTextW(Encdata);
	if (Prikey.IsEmpty()) {
		MessageBox(_T("私钥不能为空"));
		return;
	}
	if (Encdata.IsEmpty()) {
		MessageBox(_T("请输入待加密明文"));
		return;
	}
	int index = m_ketlen_cbox.GetCurSel();
	int bitlen[3] = { 512,1024,2048 };
	CString Prikey_Temp = TransData(Prikey, 1);
	C_RSA r;
	CString encpwd;
	m_encpwd.GetWindowTextW(encpwd);
	int padding = RSA_PaddingList[m_pading_cbox.GetCurSel()];
	if (r.Cstring_to_PriKey(Prikey_Temp, encpwd, m_CertType.GetCurSel()) != Success) {
		MessageBox(_T("读取私钥失败"));
		return;
	}
	//字符串解密
	if (!m_EncData.IsFile()) {
		//私钥加密格式
		CString data;
		if (r.DealData(Encdata, data, RSA_Pri_Dec, padding) < 0) {
			MessageBox(_T("解密错误"));
			return;
		}
		m_Data.SetWindowTextW(data);
	}
	//文件解密
	else {
		CFile file;
		if (file.Open(m_EncData.FilePath, CFile::modeRead) == 0) {
			MessageBox(_T("读取失败"));
			this->SetWindowTextW(_T(""));
			return;
		}
		int filesize = (int)file.GetLength();
		char filedata[1000], DealData[1000];
		memset(filedata, 0, sizeof(char) * 1000);
		memset(DealData, 0, sizeof(char) * 1000);
		file.Read(filedata, filesize);
		file.Close();
		int DealDatalen = 0;
		if (r.DealData(filedata, filesize, DealData, DealDatalen, RSA_Pri_Dec, padding) < 0) {
			MessageBox(_T("解密错误"));
			return;
		}
		//显示解密后数据
		CString filepath = FileDlg_Save(DealData, DealDatalen);
		m_Data.showFileData(filepath);
	}
	UpdateData(FALSE);
}


void CRSAPage::OnCbnSelchangeCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	//若变更到PKCS#1可以添加私钥密码对私钥进行加密
	//清空公私密钥内容
	UpdateData(TRUE);
	int index = m_CertType.GetCurSel();
	m_Privatekey.SetWindowTextW(_T(""));
	m_Publickey.SetWindowTextW(_T(""));
	//PKCS#8 隐藏私钥密码两个控件并清空
	if (index == 0) {
		m_encpwdStext.ShowWindow(SW_HIDE);
		m_encpwd.ShowWindow(SW_HIDE);
	}
	//PKCS#1 显示控件
	else {
		m_encpwdStext.ShowWindow(SW_SHOW);
		m_encpwd.ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);

}


void CRSAPage::OnBnClickedButtonOpenPri()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开私钥
	//显示数据 记录文件名
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


void CRSAPage::OnBnClickedButtonImportPri()
{
	// TODO: 在此添加控件通知处理程序代码
	//导出私钥
	UpdateData(TRUE);
	CString temp;
	m_Privatekey.GetWindowTextW(temp);
	if (temp.IsEmpty()) {
		MessageBox(_T("内容不能为空"));
		return;
	}
	//False为保存
	BOOL isOpen = FALSE;
	CString filename= _T("Prikey.pem");
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		m_Privatekey.SaveFileData(OpenFileDlg.GetPathName());
	}
	UpdateData(FALSE);
}

void CRSAPage::OnBnClickedButtonOpenPub()
{
	// TODO: 在此添加控件通知处理程序代码
	//导入公钥
	//显示数据 记录文件名
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




void CRSAPage::OnBnClickedButtonImportPub()
{
	// TODO: 在此添加控件通知处理程序代码
	//导出公钥
	UpdateData(TRUE);
	CString temp;
	m_Privatekey.GetWindowTextW(temp);
	if (temp.IsEmpty()) {
		MessageBox(_T("内容不能为空"));
		return;
	}
	//False为保存
	BOOL isOpen = FALSE;
	CString filename= _T("Pubkey.pem");
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		m_Publickey.SaveFileData(OpenFileDlg.GetPathName());
	}
	UpdateData(FALSE);
}


void CRSAPage::OnBnClickedButtonImportData()
{
	// TODO: 在此添加控件通知处理程序代码
	//导入数据
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


void CRSAPage::OnBnClickedButtonImportEncData()
{
	// TODO: 在此添加控件通知处理程序代码
	//导入数据
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


void CRSAPage::OnCbnSelchangeComboKeylen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_EncData.SetWindowTextW(_T(""));
	m_Data.SetWindowTextW(_T(""));
	UpdateData(FALSE);
}
