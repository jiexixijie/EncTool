#pragma once


// CTwofishPage 对话框

class CTwofishPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTwofishPage)

public:
	CTwofishPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTwofishPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_Twofish };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_enctype_cbox;
	CComboBox m_padding_cbox;
public:
	virtual BOOL OnInitDialog();
};
