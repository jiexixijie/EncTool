#pragma once


// CBlowfishPage 对话框

class CBlowfishPage : public CDialogEx
{
	DECLARE_DYNAMIC(CBlowfishPage)

public:
	CBlowfishPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBlowfishPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_Blowfish };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_enctype_cbox;
	CComboBox m_padding_cbox;
};
