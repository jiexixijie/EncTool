// CSetMonListPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "CSetMonListPage.h"
#include "afxdialogex.h"
#include "C_Other.h"

// CSetMonListPage 对话框

IMPLEMENT_DYNAMIC(CSetMonListPage, CDialogEx)

CSetMonListPage::CSetMonListPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_SetMonList, pParent)
{

}

CSetMonListPage::~CSetMonListPage()
{
}

void CSetMonListPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_MonFolderList_listc);
}


BEGIN_MESSAGE_MAP(CSetMonListPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetMonListPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSetMonListPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetMonListPage::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSetMonListPage 消息处理程序


void CSetMonListPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//添加监控文件路径
	UpdateData(TRUE);
	CString AddPath = GetBrowseFolderPath(this);
	if (!AddPath.IsEmpty()) {
		//将获取到的路径写入到List中
		m_MonFolderList_listc.InsertItem(m_MonFolderList_listc.GetItemCount(), AddPath);
		m_MonFolderList_listc.SetCheck(m_MonFolderList_listc.GetItemCount()-1, TRUE);
	}
	UpdateData(FALSE);
}


BOOL CSetMonListPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_MonFolderList_listc.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES); //添加复选按钮
	//读取文件
	CStdioFile  file;
	file.Open(_T("MonFloder.txt"),CFile::modeRead);
	if (file != NULL) {
		CString Path;
		int index = 0;
		while (file.ReadString(Path)) {
			//去掉换行符
			if (Path.GetAt(Path.GetLength() - 1) == '\r') {
				Path=Path.Left(Path.GetLength() - 1);
			}
			m_MonFolderList_listc.InsertItem(index, Path);
			m_MonFolderList_listc.SetCheck(index,TRUE);
			index++;
		}
		UpdateData(FALSE);
		file.Close();
	}
	
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSetMonListPage::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CStdioFile  file;
	file.Open(_T("MonFloder.txt"), CFile::modeReadWrite | CFile::modeCreate);
	if (file == NULL) {
		MessageBox(_T("无法打开MonFloder.txt"));
		return;
	}
	int nums = m_MonFolderList_listc.GetItemCount();
	for (int i = 0; i < nums; i++) {
		//如果被选中 则写入到文件中
		if (m_MonFolderList_listc.GetCheck(i)) {
			file.WriteString(m_MonFolderList_listc.GetItemText(i, 0) + _T("\r\n"));
		}
	}
	file.Close();
}


void CSetMonListPage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//更新路径
	//删除选中路径
	UpdateData(TRUE);
	for (int i = 0; i < m_MonFolderList_listc.GetItemCount(); i++)
	{
		//判断i 这一项是否选中
		if (m_MonFolderList_listc.GetCheck(i) != TRUE)
			//删除这一项
			m_MonFolderList_listc.DeleteItem(i);
	}

	//检查是否有包含项
	for (int i = 0; i < m_MonFolderList_listc.GetItemCount(); i++) {
		for (int j = i + 1; j < m_MonFolderList_listc.GetItemCount(); j++) {
			CString path = m_MonFolderList_listc.GetItemText(i,0);
			CString path2 = m_MonFolderList_listc.GetItemText(j, 0);
			if (path.Find(path2) >= 0) {
				//后面的路径被前面的包含了，则进行删去前面的
				m_MonFolderList_listc.DeleteItem(i);
				i--;
				break;

			}
			else if (path2.Find(path) >= 0) {
				//前面的路径被包含了 则长的 并退出第二个for循环
				m_MonFolderList_listc.DeleteItem(j);
				//数量减1
				j--;
			}

		}
	}
	MessageBox(_T("已去掉未选中项和包含项"));
	UpdateData(FALSE);
}
