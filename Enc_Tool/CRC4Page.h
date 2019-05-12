#pragma once
#include "MyCEdit.h"

// CRC4Page 对话框

class CRC4Page : public CDialogEx
{
	DECLARE_DYNAMIC(CRC4Page)

public:
	CRC4Page(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRC4Page();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_RC4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_key;
	MyCEdit m_Data;
	MyCEdit m_EncData;
public:
	afx_msg void OnBnClickedButtonEnc();
	afx_msg void OnBnClickedButtonDec();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonImportencdata();
	afx_msg void OnBnClickedButtonImportdata();
};
