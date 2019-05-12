#pragma once


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
};
