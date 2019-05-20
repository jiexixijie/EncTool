#pragma once
#include "CSetMonListPage.h"
#include <vector>
// COtherPage 对话框

class COtherPage : public CDialogEx
{
	DECLARE_DYNAMIC(COtherPage)

public:
	COtherPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COtherPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_Other };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_EncType_cbox;
	afx_msg void OnBnClickedButtonSetdir();
	CSetMonListPage MonListPage;
	afx_msg void OnBnClickedButtonStartmonitor();
private:
	CListCtrl m_ShowList_cctrl;
	static DWORD WINAPI MonFolder(LPVOID lParam); 
	static DWORD WINAPI StartEncFile(LPVOID lpParam);
	CButton m_StartMon_cbutton;
	CButton m_StopMon_cbutton;
	CButton m_SetDir_cbutton;
public:
	afx_msg void OnBnClickedButton1();

};
