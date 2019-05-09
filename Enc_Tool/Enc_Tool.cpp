
// Enc_Tool.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "Enc_Tool.h"
#include "Enc_ToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEncToolApp

BEGIN_MESSAGE_MAP(CEncToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEncToolApp 构造

CEncToolApp::CEncToolApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CEncToolApp 对象

CEncToolApp theApp;


// CEncToolApp 初始化

BOOL CEncToolApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CEncToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


//字符串换行符转换
CString TransData(CString A,int type) {
	//换行符转换 0x0A转0x0A0x0D
	int index = 0, start = 0;
	CString temp;
	//0:windows 转 unix 1 去\r : unix转windows \n 变 \r\n
	if (type == 0) {
		while (1)
		{
			index = A.Find(0x0A, index);
			if (index < 0) {
				break;
			}
			temp.Format(TEXT("%s%s\r"), temp, A.Mid(start, index - start));
			start = index;
			index += 1;
		}
	}
	else {
		A.Remove('\n');
		A.Replace('\r', '\n');
		temp = A;
	}
	return temp.IsEmpty()?A:temp;
}


int WriteCstringFileWC_MB(char* filename, CString data) {
	FILE* fp;
	fopen_s(&fp, filename, "wb");
	//unicode   char
	if (fp == NULL) {
		return -1;
	}
	int n = data.GetLength(); //获取str的字符数
	int flen = WideCharToMultiByte(CP_ACP, 0, data, n, NULL, 0, NULL, NULL);//获取宽字节字符的大小，大小是按字节计算的
	char* pchar = new char[flen];
	//char* pChar = new char[flen + 1]; //以字节为单位
	WideCharToMultiByte(CP_ACP, 0, data, n, pchar, flen, NULL, NULL); //宽字节编码转换成多字节编码
	int wlen = fwrite(pchar, 1, flen, fp);
	fclose(fp);
	delete[]pchar;
	return wlen==flen;
}