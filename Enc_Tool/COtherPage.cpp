// COtherPage.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "COtherPage.h"
#include "afxdialogex.h"
#include "C_Other.h"
#include "EnumList.h"

// COtherPage 对话框

std::vector<CString>EncFilelist;
std::vector<CString>BigFilelist;
std::vector<HANDLE> hThreadList;
CRITICAL_SECTION cs;

typedef struct threadshow_parameter
{
	CListCtrl* p_show;
	//CString lPath[50];
	CString Path;

}TP;


IMPLEMENT_DYNAMIC(COtherPage, CDialogEx)

COtherPage::COtherPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_Other, pParent)
{

}

COtherPage::~COtherPage()
{
}

void COtherPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EncType, m_EncType_cbox);
	DDX_Control(pDX, IDC_LIST1, m_ShowList_cctrl);
	DDX_Control(pDX, IDC_BUTTON_StartMonitor, m_StartMon_cbutton);
	DDX_Control(pDX, IDC_BUTTON1, m_StopMon_cbutton);
	DDX_Control(pDX, IDC_BUTTON_SetDir, m_SetDir_cbutton);
}


BEGIN_MESSAGE_MAP(COtherPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SetDir, &COtherPage::OnBnClickedButtonSetdir)
	ON_BN_CLICKED(IDC_BUTTON_StartMonitor, &COtherPage::OnBnClickedButtonStartmonitor)
	ON_BN_CLICKED(IDC_BUTTON1, &COtherPage::OnBnClickedButton1)
END_MESSAGE_MAP()




// COtherPage 消息处理程序


BOOL COtherPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_EncType_cbox.SetCurSel(0);
	m_StopMon_cbutton.EnableWindow(FALSE);
	UpdateData(FALSE);
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void COtherPage::OnBnClickedButtonSetdir()
{
	// TODO: 在此添加控件通知处理程序代码
	MonListPage.DoModal();
}


void COtherPage::OnBnClickedButtonStartmonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	//开始监控
	//无法被选中
	m_StartMon_cbutton.EnableWindow(FALSE);
	m_SetDir_cbutton.EnableWindow(FALSE);
	m_StopMon_cbutton.EnableWindow(TRUE);

	//输出系统时间
	CString str;   //获取系统时间
	CTime tm;
	tm = CTime::GetCurrentTime();
	str = tm.Format("%Y年%m月%d日 %X");
	CString str1 = L"================= 监控已开启 " + str + L"=================";
	m_ShowList_cctrl.InsertItem(m_ShowList_cctrl.GetItemCount(), str1);

	//输出监控目录
	CStdioFile sf;
	CString content;
	std::vector<CString> PathList;
	m_ShowList_cctrl.InsertItem(m_ShowList_cctrl.GetItemCount(), L"正在监控的目录为");
	if (sf.Open(_T("MonFloder.txt"), CFile::modeRead ))
	{
		while (sf.ReadString(content))
		{
			if (content .IsEmpty())
				continue;
			//去掉换行符
			if (content.GetAt(content.GetLength() - 1) == '\r') {
				content = content.Left(content.GetLength() - 1);
			}
			m_ShowList_cctrl.InsertItem(m_ShowList_cctrl.GetItemCount(), content);
			PathList.insert(PathList.end(),content);
		}
		sf.Close();
	}

	//创建线程监控每隔文件路径
	DWORD ThreadID;
	int Listsize = PathList.size();
	InitializeCriticalSection(&cs);//初始化临界区
	//DeleteCriticalSection(&cs);//删除临界区
	for (int i = 0; i < Listsize; i++) {
		TP *params=new TP;
		params->p_show = &m_ShowList_cctrl;
		params->Path = PathList[i];
		HANDLE h=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonFolder, (LPVOID)params, 0, &ThreadID);
		hThreadList.push_back(h);
		TRACE("Thread:%d Start\n", ThreadID);
	}
	HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartEncFile, (void *)this, 0, &ThreadID);
	hThreadList.push_back(h);
}



DWORD WINAPI COtherPage::MonFolder(LPVOID lParam)
{
	TP* data = (TP*)lParam;
	CListCtrl* p = data->p_show;
	CString Path = data->Path;
	BOOL bRet = FALSE;
	BYTE Buffer[1024] = { 0 };
	int line = 0;
	FILE_NOTIFY_INFORMATION* pBuffer = (FILE_NOTIFY_INFORMATION*)Buffer;
	DWORD dwByteReturn = 0;

	HANDLE hFile = ::CreateFile(Path,
		FILE_LIST_DIRECTORY,FILE_SHARE_READ |
		FILE_SHARE_WRITE |
		FILE_SHARE_DELETE,NULL,
		OPEN_EXISTING, 
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		int n = GetLastError();
		TRACE("CreateFile Error!\n");
		return 1;
	}

	TRACE("monitoring...\r\n");
	p->InsertItem(p->GetItemCount(), L"monitoring...\r\n");
	while (TRUE)
	{
		::ZeroMemory(Buffer, sizeof(Buffer));
		bRet = ::ReadDirectoryChangesW(hFile,
			&Buffer,
			sizeof(Buffer),
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |			// 修改文件名
			FILE_NOTIFY_CHANGE_ATTRIBUTES |			// 修改文件属性
			FILE_NOTIFY_CHANGE_LAST_WRITE,			// 最后一次写入
			&dwByteReturn, NULL, NULL);
		if (TRUE == bRet)
		{
			//输出系统时间
			CString str;   //获取系统时间
			CTime tm;
			tm = CTime::GetCurrentTime();
			str = tm.Format("%Y年%m月%d日 %X");
			char szFileName[MAX_PATH] = { 0 };
			// 宽字符转换为多字节
			::WideCharToMultiByte(CP_ACP,
				0,
				pBuffer->FileName,
				(pBuffer->FileNameLength / 2),
				szFileName,
				MAX_PATH,
				NULL,
				NULL);
			if (pBuffer->Action == FILE_ACTION_ADDED) {
				//显示添加
				CString CFileName(szFileName);
				//表示已经被加密了则跳过
				if (CFileName.Find(_T("EncTool")) > 0) {
					continue;
				}
				p->InsertItem(p->GetItemCount(), str + L"添加:\t" + Path + "\\" + CFileName);
				//将路径添加到待加密列表中
				
				EnterCriticalSection(&cs);
				//可以添加判断文件类型
				EncFilelist.insert(EncFilelist.end(), Path + "\\" + CFileName);
				LeaveCriticalSection(&cs);
			}
		}
	}
	CloseHandle(hFile);
	//LeaveCriticalSection(&cs);
	return 0;
}


DWORD WINAPI COtherPage::StartEncFile(LPVOID lpParam){
	COtherPage* p = (COtherPage*)lpParam;
	int EncType[] = { Enc_AES,Enc_DES,Enc_RC4 };
	int type = EncType[p->m_EncType_cbox.GetCurSel()];
	while (true){
		if (EncFilelist.empty()) {
			//等待1秒
			Sleep(1000);
			continue;
		}
		if (type == Enc_AES) {    //AES
			std::vector<CString>::iterator it = EncFilelist.begin();
			if (EncFile_AES(*it) != Success) {
				continue;
			}
			EnterCriticalSection(&cs);
			//显示信息
			CString str;   //获取系统时间
			CTime tm;
			tm = CTime::GetCurrentTime();
			str = tm.Format("%Y年%m月%d日 %X");
			CListCtrl* pshow = &(p->m_ShowList_cctrl);
			pshow->InsertItem(pshow->GetItemCount(), str + L"加密成功:\t" + *it);
			//从列表中除去
			EncFilelist.erase(it);
			LeaveCriticalSection(&cs);

		}
		else if (type == Enc_DES) {  //DES
			
		}
		else {                    //RC4

		}
	}
}

void COtherPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;   //获取系统时间
	CTime tm;
	tm = CTime::GetCurrentTime();
	str = tm.Format("%Y年%m月%d日 %X");
	//MessageBox(str,NULL,MB_OK);
	CString str1 = L"================= 监控已停止 " + str + L"=================";
	m_ShowList_cctrl.InsertItem(m_ShowList_cctrl.GetItemCount(), str1);
	//::WaitForSingleObject(hThread, INFINITE);
	//PostNcDestroy();
	::DestroyThreadpoolEnvironment((PTP_CALLBACK_ENVIRON)MonFolder);
	::DestroyThreadpoolEnvironment((PTP_CALLBACK_ENVIRON)StartEncFile);

	for (int i = 1; i < hThreadList.size(); i++) {
		TerminateThread(hThreadList[i], -1);
		Sleep(100);
		CloseHandle(hThreadList[i]);
	}
	m_StopMon_cbutton.EnableWindow(FALSE);
	m_StartMon_cbutton.EnableWindow(TRUE);
	m_SetDir_cbutton.EnableWindow(TRUE);
}
