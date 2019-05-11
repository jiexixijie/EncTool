#pragma once
#include "MyCEdit.h"

// C3DESPage 对话框

class C3DESPage : public CDialogEx
{
	DECLARE_DYNAMIC(C3DESPage)

public:
	C3DESPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~C3DESPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_3DES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_key;
	CStatic m_ivText;
	CEdit m_iv;
	CComboBox m_EncType_cbox;
	CComboBox m_padding_cbox;
	MyCEdit m_Data;
	MyCEdit m_EncData;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonEnc();
	afx_msg void OnBnClickedButtonDec();
	afx_msg void OnBnClickedButtonImportdata();
	afx_msg void OnBnClickedButtonImportencdata();
};
