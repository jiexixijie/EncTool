#pragma once


// CRSAPagePri 对话框

class CRSAPagePri : public CDialogEx
{
	DECLARE_DYNAMIC(CRSAPagePri)

public:
	CRSAPagePri(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRSAPagePri();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_RSA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
