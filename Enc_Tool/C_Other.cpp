#include "pch.h"
#include "C_Other.h"
#include "C_RC4.h"
#include "C_RSA.h"
#include "C_DES.h"
#include "C_AES.h"
#include <time.h>
#include <vector>
C_RSA r;
//�ж�RSA r�Ƿ��Ѿ���ȡ��Կ
bool RSA_Read = FALSE;
extern std::vector<CString>BigFilelist;  //���ڴ洢���ļ�



CString GetBrowseFolderPath(CWnd* pWnd) {
	wchar_t szFolderName[MAX_PATH] = { 0 }; //�洢�ļ��е�����
	wchar_t szFolderPath[MAX_PATH] = { 0 }; //�洢�ļ��е�·��
	BROWSEINFO info;
	info.hwndOwner = pWnd->m_hWnd; //�ĸ�����ӵ������Ի���
	info.pidlRoot = NULL; //NULL�Ǵ������ļ��п�ʼ���
	info.pszDisplayName = szFolderName; //�����ļ��е�����
	info.lpszTitle = L"��ѡ��Ҫ��ص��ļ��У�"; //����
	info.ulFlags = NULL; //��Ӷ���Ŀؼ�
	info.lpfn = NULL;
	info.lParam = NULL;
	info.iImage = NULL;

	PCIDLIST_ABSOLUTE pa = ::SHBrowseForFolder(&info); //��ʾһ������ļ��еĶԻ���

	if (::SHGetPathFromIDList(pa, (LPWSTR)szFolderPath) == TRUE)
	{
		int a = 100;
		if (wcscmp(szFolderPath, L"") != 0) //��ȡ�ɹ���
		{
			return szFolderPath;
		}
	}
	return L"";
}

//������ɳ���Ϊlen���ַ���
void GenRandomKey(BYTE* key, int len) {
	srand((unsigned int)time(NULL));
	//�������0-0xFF����
	for (int i = 0; i < len; i++) {
		key[i] = rand() % 0x100;
	}
}

int EncFile_AES(CString FilePath) {
	EncFileHead head;
	memset(&head, 0, sizeof(EncFileHead));
	//���ñ�־
	//��·���л�ȡ�ļ����ƺ��ļ���ʽ
	int nPos = FilePath.ReverseFind('\\');
	CString FileName = FilePath.Right(FilePath.GetLength() - nPos - 1);
	nPos = FileName.ReverseFind('.');
	CString FileType = FileName.Right(FileName.GetLength() - nPos - 1);
	FileName = FileName.Left(nPos);


	//ʹ��AES_128_CBC;
	memcpy_s(head.Flag,8,"EncTool",7);   //д��FLAG
	//���ֻ��д��47���ַ�
	WideCharToMultiByte(CP_ACP, 0, FileName, FileName.GetLength(), head.filename, 47, NULL, NULL);//д��FILANAME
	WideCharToMultiByte(CP_ACP, 0, FileType, FileType.GetLength(), head.fileType, 7, NULL, NULL);//д��FILETYPE
	head.EncType=Enc_AES;//д��ENCTYPE

	//���ļ��ж�ȡ��˽��Կ��
	if (!RSA_Read) {
		if (r.ReadPubKey("Pubkey.pem", PKCS8) != Success) {
			AfxMessageBox(_T("��ȡ��Կʧ��"));
			return InitError;
		}
		else {
			RSA_Read = TRUE;
		}
	}
	
	//����128λ����Կ 
	char key[256];
	GenRandomKey((BYTE*)key, 16);
	int EncKeylen=0;
	if (r.DealData(key, 16, head.EncKeyData, EncKeylen, RSA_Pub_Enc, RSA_PKCS1_PADDING) != Success) { //д��ENCKEYDATA
		AfxMessageBox(_T("����ʧ��"));
		return Enc_Dec_Fail;
	}
	head.keylen = 16;//д��KEY LEN


	TCHAR szTempFileName[MAX_PATH];
	GetTempPath(MAX_PATH, szTempFileName);
	CString tempname = FilePath; tempname.Remove('\\'); tempname.Remove(':');
	CString WTempFilePath = szTempFileName + tempname + "_EncTool";
	//if (filesize > 1024 * 1024 * 200) {
	//	//���ļ��Ժ���
	//	BigFilelist.insert(BigFilelist.end(), FilePath);
	//	return Success;
	//}
	//ʹ��AES�����ļ�
	C_AES a;
	a.SetKeySize(16);
	char iv[16];
	memset(iv, 0, 16);
	strcpy_s(iv, 16, "233");
	int result=a.EncFile(FilePath, WTempFilePath, key, iv, CBC);
	switch (result)
	{
	case Success:
		//ɾ��ԭ�ļ� �滻Ϊ�����ļ�
		CFile::Remove(FilePath);
		CFile::Rename(WTempFilePath, FilePath + "EncTool");
		return Success;
	case ReadError:
		AfxMessageBox(_T("�ļ���ȡʧ��"));
		return ReadError;
	case WriteError:
		AfxMessageBox(_T("�ļ�д��ʧ��"));
		return WriteError;
	default:
		AfxMessageBox(_T("����ʧ��"));
		return Enc_Dec_Fail;
	}
}