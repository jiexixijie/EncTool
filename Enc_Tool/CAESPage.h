#pragma once


// CAESPage 对话框

class CAESPage : public CDialogEx
{
	DECLARE_DYNAMIC(CAESPage)

public:
	CAESPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAESPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_AES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_enctype_cbox;
	CComboBox m_paddingtype_cbox;
	CComboBox m_blocksize_cbox;
public:
	virtual BOOL OnInitDialog();
};
