#include "pch.h"
#include "C_Other.h"
#include "C_RC4.h"
#include "C_RSA.h"
#include "C_DES.h"
#include "C_AES.h"
#include <time.h>
#include <vector>
C_RSA r;
//判断RSA r是否已经读取公钥
bool RSA_Read = FALSE;
extern std::vector<CString>BigFilelist;  //用于存储大文件



CString GetBrowseFolderPath(CWnd* pWnd) {
	wchar_t szFolderName[MAX_PATH] = { 0 }; //存储文件夹的名字
	wchar_t szFolderPath[MAX_PATH] = { 0 }; //存储文件夹的路径
	BROWSEINFO info;
	info.hwndOwner = pWnd->m_hWnd; //哪个窗口拥有这个对话框
	info.pidlRoot = NULL; //NULL是从桌面文件夹开始浏览
	info.pszDisplayName = szFolderName; //接收文件夹的名字
	info.lpszTitle = L"请选择要监控的文件夹："; //标题
	info.ulFlags = NULL; //添加额外的控件
	info.lpfn = NULL;
	info.lParam = NULL;
	info.iImage = NULL;

	PCIDLIST_ABSOLUTE pa = ::SHBrowseForFolder(&info); //显示一个浏览文件夹的对话框

	if (::SHGetPathFromIDList(pa, (LPWSTR)szFolderPath) == TRUE)
	{
		int a = 100;
		if (wcscmp(szFolderPath, L"") != 0) //获取成功了
		{
			return szFolderPath;
		}
	}
	return L"";
}

//随机生成长度为len的字符串
void GenRandomKey(BYTE* key, int len) {
	srand((unsigned int)time(NULL));
	//随机生成0-0xFF数据
	for (int i = 0; i < len; i++) {
		key[i] = rand() % 0x100;
	}
}

int EncFile_AES(CString FilePath) {
	EncFileHead head;
	memset(&head, 0, sizeof(EncFileHead));
	//设置标志
	//从路径中获取文件名称和文件格式
	int nPos = FilePath.ReverseFind('\\');
	CString FileName = FilePath.Right(FilePath.GetLength() - nPos - 1);
	nPos = FileName.ReverseFind('.');
	CString FileType = FileName.Right(FileName.GetLength() - nPos - 1);
	FileName = FileName.Left(nPos);


	//使用AES_128_CBC;
	memcpy_s(head.Flag,8,"EncTool",7);   //写入FLAG
	//最多只能写入47个字符
	WideCharToMultiByte(CP_ACP, 0, FileName, FileName.GetLength(), head.filename, 47, NULL, NULL);//写入FILANAME
	WideCharToMultiByte(CP_ACP, 0, FileType, FileType.GetLength(), head.fileType, 7, NULL, NULL);//写入FILETYPE
	head.EncType=Enc_AES;//写入ENCTYPE

	//从文件中读取公私密钥对
	if (!RSA_Read) {
		if (r.ReadPubKey("Pubkey.pem", PKCS8) != Success) {
			AfxMessageBox(_T("读取公钥失败"));
			return InitError;
		}
		else {
			RSA_Read = TRUE;
		}
	}
	
	//生成128位的密钥 
	char key[256];
	GenRandomKey((BYTE*)key, 16);
	int EncKeylen=0;
	if (r.DealData(key, 16, head.EncKeyData, EncKeylen, RSA_Pub_Enc, RSA_PKCS1_PADDING) != Success) { //写入ENCKEYDATA
		AfxMessageBox(_T("加密失败"));
		return Enc_Dec_Fail;
	}
	head.keylen = 16;//写入KEY LEN


	TCHAR szTempFileName[MAX_PATH];
	GetTempPath(MAX_PATH, szTempFileName);
	CString tempname = FilePath; tempname.Remove('\\'); tempname.Remove(':');
	CString WTempFilePath = szTempFileName + tempname + "_EncTool";
	//if (filesize > 1024 * 1024 * 200) {
	//	//大文件稍后处理
	//	BigFilelist.insert(BigFilelist.end(), FilePath);
	//	return Success;
	//}
	//使用AES加密文件
	C_AES a;
	a.SetKeySize(16);
	char iv[16];
	memset(iv, 0, 16);
	strcpy_s(iv, 16, "233");
	int result=a.EncFile(FilePath, WTempFilePath, key, iv, CBC);
	switch (result)
	{
	case Success:
		//删除原文件 替换为加密文件
		CFile::Remove(FilePath);
		CFile::Rename(WTempFilePath, FilePath + "EncTool");
		return Success;
	case ReadError:
		AfxMessageBox(_T("文件读取失败"));
		return ReadError;
	case WriteError:
		AfxMessageBox(_T("文件写入失败"));
		return WriteError;
	default:
		AfxMessageBox(_T("解密失败"));
		return Enc_Dec_Fail;
	}
}