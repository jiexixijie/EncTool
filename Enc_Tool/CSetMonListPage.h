#pragma once


// CSetMonListPage 对话框

class CSetMonListPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSetMonListPage)

public:
	CSetMonListPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetMonListPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_SetMonList };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	CListCtrl m_MonFolderList_listc;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
