
// Enc_ToolDlg.h: 头文件
//

#pragma once
#include "CRSAPage.h"   
#include "CAESPage.h"
#include "CDESPage.h"
#include "C3DESPage.h"
#include "CBlowfishPage.h"
#include "CRC4Page.h"
#include "CHashPage.h"
#include "COtherPage.h"
#include "CSignVerifyPage.h"
// CEncToolDlg 对话框
class CEncToolDlg : public CDialogEx
{
// 构造
public:
	CEncToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENC_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tab;
	CRSAPage RSApage;
	CAESPage AESpage;
	CDESPage DESpage;
	C3DESPage DES3page;
	CBlowfishPage Blowfishpage;
	CRC4Page RC4page;
	CHashPage Hashpage;
	CSignVerifyPage SignVerifypage;
	COtherPage Otherpage;
	//保存页面 方便进行显示和隐藏
	CDialog* pages[10];
	int m_CurSelTab;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
