#pragma once


// CSignVerifyPage 对话框

class CSignVerifyPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSignVerifyPage)

public:
	CSignVerifyPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSignVerifyPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_SignVerify };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_hashtype_cbox;
public:
	virtual BOOL OnInitDialog();
};
