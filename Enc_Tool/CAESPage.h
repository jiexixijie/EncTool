#pragma once

#include "MyCEdit.h"


// CAESPage 对话框

class CAESPage : public CDialogEx
{
	DECLARE_DYNAMIC(CAESPage)

public:
	CAESPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAESPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_AES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	MyCEdit m_Data;
	MyCEdit m_EncData;
	CComboBox m_paddding_cbox;
	CComboBox m_EncType_cbox;
	CString m_key;
	CEdit m_iv;
	CButton m_ivText;
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_KeySize_cbox;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonImportdata();
	afx_msg void OnBnClickedButtonImportencdata();
	afx_msg void OnCbnSelchangeCombo2();
};
