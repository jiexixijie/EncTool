// CBlowfishPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CBlowfishPage.h"
#include "afxdialogex.h"
#include "C_Blowfish.h"
#include <openssl/evp.h>


int Blowfish_TYPE_List[] = { CBC,ECB, OFB, CFB };
// CBlowfishPage 对话框

IMPLEMENT_DYNAMIC(CBlowfishPage, CDialogEx)

CBlowfishPage::CBlowfishPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_Blowfish, pParent)
	, m_key(_T(""))
{

}

CBlowfishPage::~CBlowfishPage()
{
}

void CBlowfishPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Key, m_key);
	DDX_Control(pDX, IDC_IVText, m_ivText);
	DDX_Control(pDX, IDC_EDIT_iv, m_iv);
	DDX_Control(pDX, IDC_COMBO_PaddingType, m_padding_cbox);
	DDX_Control(pDX, IDC_COMBO_EncType, m_EncType_cbox);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
}


BEGIN_MESSAGE_MAP(CBlowfishPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Enc, &CBlowfishPage::OnBnClickedButtonEnc)
	ON_BN_CLICKED(IDC_BUTTON_Dec, &CBlowfishPage::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_ImportData, &CBlowfishPage::OnBnClickedButtonImportdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportEncData, &CBlowfishPage::OnBnClickedButtonImportencdata)
	ON_CBN_SELCHANGE(IDC_COMBO_EncType, &CBlowfishPage::OnCbnSelchangeComboEnctype)
END_MESSAGE_MAP()


// CBlowfishPage 消息处理程序


BOOL CBlowfishPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EncType_cbox.SetCurSel(0);
	//填充模式
	m_padding_cbox.SetCurSel(0);
	m_key = "1111";
	m_iv.SetWindowTextW(_T("12345678"));
	m_Data.SetWindowTextW(_T("Welcome"));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CBlowfishPage::OnBnClickedButtonEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密
	//加密 
	//检查密钥长度不超过8字节
	UpdateData(TRUE);
	if (m_key.GetLength() < 4 || m_key.GetLength() > 56) {
		MessageBox(_T("密钥长度应为32bits-448bits"));
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
	char key_c[56], iv_c[8];
	memset(key_c, 0, 56);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_Blowfish b;
	//字符串加密 BASE64显示
	if (!m_Data.IsFile()) {
		CString EncData;
		//windows换行符为'/r/n' 转换为'/n'
		Data = TransData(Data, 1);
		if (b.EncData(Data, EncData, key_c, iv_c, Blowfish_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
		CString FileR_path = m_Data.FilePath;
		if (FileR_path == FileW_path) {
			MessageBox(_T("两个文件应不同"));
			return;
		}
		int result = b.EncFile(FileR_path, FileW_path, key_c, iv_c, Blowfish_TYPE_List[m_EncType_cbox.GetCurSel()]);
		switch (result)
		{
		case ReadError:
			MessageBox(_T("文件读取失败"));
			m_EncData.EmptyFile();
			break;
		case WriteError:
			MessageBox(_T("文件写入失败"));
			m_EncData.EmptyFile();
			break;
		case Success:
			//显示加密后密文
			m_EncData.showFileHexData(FileW_path);
			break;
		default:
			MessageBox(_T("加密失败"));
			m_EncData.EmptyFile();
		}
	}
	UpdateData(FALSE);

}


void CBlowfishPage::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_key.GetLength() < 4 || m_key.GetLength() > 56) {
		MessageBox(_T("密钥长度应为32bits-448bits"));
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
	char key_c[56], iv_c[8];
	memset(key_c, 0, 56);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_Blowfish b;
	//字符串解密 BASE64显示
	if (!m_EncData.IsFile()) {
		CString Data;
		if (b.DecData(EncData, Data, key_c, iv_c, Blowfish_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
		CString FileR_path = m_EncData.FilePath;
		if (m_EncData.FilePath == FileW_path) {
			MessageBox(_T("两个文件应不同"));
			return;
		}
		int result = b.DecFile(FileR_path, FileW_path, key_c, iv_c, Blowfish_TYPE_List[m_EncType_cbox.GetCurSel()]);
		switch (result)
		{
		case ReadError:
			MessageBox(_T("文件读取失败"));
			m_Data.EmptyFile();
			break;
		case WriteError:
			MessageBox(_T("文件写入失败"));
			m_Data.EmptyFile();
			break;
		case Success:
			m_Data.showFileData(FileW_path);
			break;
		default:
			MessageBox(_T("解密失败"));
			m_Data.EmptyFile();
		}
	}
	UpdateData(FALSE);

}


void CBlowfishPage::OnBnClickedButtonImportdata()
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


void CBlowfishPage::OnBnClickedButtonImportencdata()
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


void CBlowfishPage::OnCbnSelchangeComboEnctype()
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
