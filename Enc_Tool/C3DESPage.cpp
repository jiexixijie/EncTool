// C3DESPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "C3DESPage.h"
#include "afxdialogex.h"
#include "C_3DES.h"
#include <openssl/evp.h>

int DES3_TYPE_List[] = { CBC,ECB, OFB, CFB };

// C3DESPage 对话框

IMPLEMENT_DYNAMIC(C3DESPage, CDialogEx)

C3DESPage::C3DESPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_3DES, pParent)
	, m_key(_T(""))
{

}

C3DESPage::~C3DESPage()
{
}

void C3DESPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Key, m_key);
	DDX_Control(pDX, IDC_IVText, m_ivText);
	DDX_Control(pDX, IDC_EDIT_iv, m_iv);
	DDX_Control(pDX, IDC_COMBO_EncType, m_EncType_cbox);
	DDX_Control(pDX, IDC_COMBO_PaddingType, m_padding_cbox);
	DDX_Control(pDX, IDC_EDIT_Data, m_Data);
	DDX_Control(pDX, IDC_EDIT_EncData, m_EncData);
}


BEGIN_MESSAGE_MAP(C3DESPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Enc, &C3DESPage::OnBnClickedButtonEnc)
	ON_BN_CLICKED(IDC_BUTTON_Dec, &C3DESPage::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_ImportData, &C3DESPage::OnBnClickedButtonImportdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportEncData, &C3DESPage::OnBnClickedButtonImportencdata)
	ON_CBN_SELCHANGE(IDC_COMBO_EncType, &C3DESPage::OnCbnSelchangeComboEnctype)
END_MESSAGE_MAP()


// C3DESPage 消息处理程序


BOOL C3DESPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_iv.SetWindowTextW(_T("12345678"));
	m_Data.SetWindowTextW(_T("Welcome"));
	m_padding_cbox.SetCurSel(0);
	m_EncType_cbox.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void C3DESPage::OnBnClickedButtonEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密密文
	// iv不超过8位 不足8位末尾补齐0
	//key不超过24位 8*3 不足补齐0
	UpdateData(TRUE);
	if (m_key.GetLength() > 24) {
		MessageBox(_T("密钥长度不能超过24Bytes"));
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
	char key_c[24], iv_c[8];
	memset(key_c, 0, 24);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_3DES d;
	//字符串加密 BASE64显示
	if (!m_Data.IsFile()) {
		CString EncData;
		//windows换行符为'/r/n' 转换为'/n'
		Data = TransData(Data, 1);
		if (d.EncData(Data, EncData, key_c, iv_c, DES3_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
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
		int result = d.EncFile(FileR_path, FileW_path, key_c, iv_c, DES3_TYPE_List[m_EncType_cbox.GetCurSel()]);
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


void C3DESPage::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_key.GetLength() > 24) {
		MessageBox(_T("密钥长度不能超过24Bytes"));
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
	char key_c[24], iv_c[8];
	memset(key_c, 0, 24);
	memset(iv_c, 0, 8);
	Get_Key_Vi_c(m_key, key_c, iv, iv_c);
	C_3DES d;
	//字符串解密 BASE64显示
	if (!m_EncData.IsFile()) {
		CString Data;
		if (d.DecData(EncData, Data, key_c, iv_c, DES3_TYPE_List[m_EncType_cbox.GetCurSel()]) != Success) {
			m_Data.EmptyFile();
			MessageBox(_T("解密失败"));
		}
		Data = TransData(Data, 0);
		m_Data.SetWindowTextW(Data);
	}
	else {
		CFile fileR, fileW;
		CString FileW_path = FileDlg_GetSavePath();
		CString FileR_path = m_EncData.FilePath;
		if (m_EncData.FilePath == FileW_path) {
			MessageBox(_T("两个文件应不同"));
			return;
		}
		int result = d.DecFile(FileR_path, FileW_path, key_c, iv_c, DES3_TYPE_List[m_EncType_cbox.GetCurSel()]);
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




void C3DESPage::OnBnClickedButtonImportdata()
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


void C3DESPage::OnBnClickedButtonImportencdata()
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


void C3DESPage::OnCbnSelchangeComboEnctype()
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
