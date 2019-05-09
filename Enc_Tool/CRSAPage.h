#pragma once
#include "MyCEdit.h"

// CRSAPage 对话框

class CRSAPage : public CDialog
{
	DECLARE_DYNAMIC(CRSAPage)

public:
	CRSAPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRSAPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_RSA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// 判读指数进制 
	CComboBox m_HexDec_cbox;
	CComboBox m_pading_cbox;
	// 设置随机生成的密钥长度
	CComboBox m_ketlen_cbox;
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonGenkey();
	afx_msg void OnBnClickedButtonImportPri();
	afx_msg void OnBnClickedButtonPubEnc();
	afx_msg void OnBnClickedButtonPriEnc();
	afx_msg void OnBnClickedButtonPubDec();
	afx_msg void OnBnClickedButtonPriDec();
	afx_msg void OnCbnSelchangeCombo6();
private:
	//MyCEdit 重载文件拖拽
	// E的值
	CString m_Evalue;
	MyCEdit m_Privatekey;
	MyCEdit m_Publickey;
	MyCEdit m_Data;
	MyCEdit m_EncData;
	CEdit m_encpwd;
	// 显示的密钥格式
	CComboBox m_CertType;
	CStatic m_encpwdStext;
public:
	afx_msg void OnBnClickedButtonOpenPri();
	afx_msg void OnBnClickedButtonOpenPub();
	afx_msg void OnBnClickedButtonImportPub();
	afx_msg void OnBnClickedButtonImportData();
	afx_msg void OnBnClickedButtonImportEncData();
	afx_msg void OnCbnSelchangeComboKeylen();
};
