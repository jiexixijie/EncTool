#pragma once
#include "MyCEdit.h"

// CHashPage 对话框

class CHashPage : public CDialogEx
{
	DECLARE_DYNAMIC(CHashPage)

public:
	CHashPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHashPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_Hash };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	MyCEdit m_Data;

public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
private:
	CString m_MD5Data;
	CString m_SHA1Data;
	CString m_SHA256Data;
};
