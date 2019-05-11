// CDESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CDESPage.h"
#include "afxdialogex.h"
#include "C_DES.h"
#include <openssl/evp.h>

//NoPadding
int DES_TYPE_List[] = { CBC,ECB, OFB, CFB };

// CDESPage 对话框

IMPLEMENT_DYNAMIC(CDESPage, CDialogEx)

CDESPage::CDESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_DES, pParent)
	, m_key(_T(""))
{

}

CDESPage::~CDESPage()
{
}

void CDESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EncType, m_enctype_cbox);
	DDX_Control(pDX, IDC_COMBO_PaddingType, m_padding_cbox);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
	DDX_Text(pDX, IDC_EDIT_Key, m_key);
	DDX_Control(pDX, IDC_IVText, m_ivText);
	DDX_Control(pDX, IDC_EDIT_iv, m_iv);
}


BEGIN_MESSAGE_MAP(CDESPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Enc, &CDESPage::OnBnClickedButtonEnc)
	ON_BN_CLICKED(IDC_BUTTON_Dec, &CDESPage::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_ImportData, &CDESPage::OnBnClickedButtonImportdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportEncData, &CDESPage::OnBnClickedButtonImportencdata)
	ON_CBN_SELCHANGE(IDC_COMBO_EncType, &CDESPage::OnCbnSelchangeComboEnctype)
END_MESSAGE_MAP()


// CDESPage 消息处理程序


void CDESPage::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}


BOOL CDESPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认为CBC模式
	m_enctype_cbox.SetCurSel(0);
	//填充模式
	m_padding_cbox.SetCurSel(0);
	m_key = "11111111";
	m_iv.SetWindowTextW(_T("12345678"));
	m_Data.SetWindowTextW(_T("Welcome"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDESPage::OnBnClickedButtonEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密 
	//检查密钥长度不超过8字节
	UpdateData(TRUE);
	if (m_key.GetLength() > 8) {
		MessageBox(_T("密钥长度不能超过8Bytes"));
		return;
	}
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	//检查iv 不超过8字节
	CString iv;
	m_iv.GetWindowTextW(iv);
	if (iv.GetLength() > 8) {
		MessageBox(_T("iv长度不能超过8Bytes"));
		return;
	}
	//判断明文不为空
	CString Data;
	m_Data.GetWindowTextW(Data);
	if (Data.IsEmpty()) {
		MessageBox(_T("请输入待加密数据"));
		return;
	}
	//将key和iv转化为char 同时如果不足8字符用'\0'末尾进行补充
	//同时若iv为空则设置为NULL 使用C_DES 默认iv:12345678
	char key_c[8], iv_c[8];
	memset(key_c, 0, 8);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_DES d;
	//字符串加密 BASE64显示
	if (!m_Data.IsFile()) {
		CString EncData;
		//windows换行符为'/r/n' 转换为'/n'
		Data=TransData(Data, 1);
		if (d.EncData(Data, EncData, key_c, iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()])!=Success) {
			MessageBox(_T("加密失败"));
		}
		else {
			m_EncData.SetWindowTextW(EncData);
		}
	}
	//文件加解密
	else {
		CFile fileR,fileW;
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
		C_DES d;
		while (Readsize<filesize){
			fileR.Read(filedata, 1024);
			if (d.EncData(filedata, 1024, CipherData, cipherlen, key_c, iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()]) != Success) {
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
			if (d.EncData(filedata, filesize, CipherData, cipherlen, key_c, iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()]) != Success) {
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


void CDESPage::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	//解密
	UpdateData(TRUE);
	if (m_key.GetLength() > 8) {
		MessageBox(_T("密钥长度不能超过8Bytes"));
		return;
	}
	if (m_key.IsEmpty()) {
		MessageBox(_T("密钥不能为空"));
		return;
	}
	//检查iv 不超过8字节
	CString iv;
	m_iv.GetWindowTextW(iv);
	if (iv.GetLength() > 8) {
		MessageBox(_T("iv长度不能超过8Bytes"));
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
	char key_c[8], iv_c[8];
	memset(key_c, 0, 8);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_DES d;
	//字符串解密 BASE64显示
	if (!m_EncData.IsFile()) {
		CString Data;
		if (d.DecData(EncData, Data, key_c,iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()]) != Success) {
			MessageBox(_T("解密失败"));
		}
		else {
			Data = TransData(Data, 0);
			m_Data.SetWindowTextW(Data);
		}
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
		C_DES d;
		while (Readsize < filesize) {
			fileR.Read(filedata, 1024);
			if (d.DecData(filedata, 1024, DecData, DecDatalen, key_c, iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()]) != Success) {
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
			if (d.DecData(filedata, filesize, DecData, DecDatalen, key_c, iv_c, DES_TYPE_List[m_enctype_cbox.GetCurSel()]) != Success) {
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


void CDESPage::OnBnClickedButtonImportdata()
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


void CDESPage::OnBnClickedButtonImportencdata()
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


void CDESPage::OnCbnSelchangeComboEnctype()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFB，OFB不要偏移量
	UpdateData(TRUE);
	int index = m_enctype_cbox.GetCurSel();
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


