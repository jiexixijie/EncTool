#pragma once
#include "MyCEdit.h"

// CSignVerifyPage 对话框

class CSignVerifyPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSignVerifyPage)

public:
	CSignVerifyPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSignVerifyPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_SignVerify };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_keylen_cbox;
	CComboBox m_CertType_cbox;
	CEdit m_encpwd;
	MyCEdit m_Privatekey;
	MyCEdit m_Publickey;
	MyCEdit m_Data;
	MyCEdit m_EncData;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
private:
	CComboBox m_padding_cbox;
	CComboBox m_HashType_cbox;
public:
	virtual BOOL OnInitDialog();
private:
	CStatic m_EncpwdStext;
public:
	afx_msg void OnBnClickedButtonGenkey();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeComboCerttype();
};
