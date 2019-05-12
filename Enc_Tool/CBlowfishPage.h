#pragma once
#include "MyCEdit.h"

// CBlowfishPage 对话框

class CBlowfishPage : public CDialogEx
{
	DECLARE_DYNAMIC(CBlowfishPage)

public:
	CBlowfishPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBlowfishPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_Blowfish };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_key;
	CStatic m_ivText;
	CEdit m_iv;
	CComboBox m_padding_cbox;
	CComboBox m_EncType_cbox;
	MyCEdit m_Data;
	MyCEdit m_EncData;
public:
	afx_msg void OnBnClickedButtonEnc();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDec();
	afx_msg void OnBnClickedButtonImportdata();
	afx_msg void OnBnClickedButtonImportencdata();
	afx_msg void OnCbnSelchangeComboEnctype();
};
