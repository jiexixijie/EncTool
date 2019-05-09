#pragma once


// CDESPage 对话框

class CDESPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDESPage)

public:
	CDESPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDESPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_DES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 加解密模式
	CComboBox m_enctype_cbox;
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_padding_cbox;
};
