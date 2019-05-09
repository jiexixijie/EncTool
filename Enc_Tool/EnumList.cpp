#include "pch.h"
#include <afx.h>
#include <string>
#include "EnumList.h"

//BASE64  64���ַ�
const char* ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

void EncBase64(char* charBuf, int charBufLen, char* base64Char, int& base64CharLen)
{
	int a = 0;
	int i = 0;
	while (i<charBufLen){
		char b0 = charBuf[i++];
		char b1 = (i < charBufLen) ? charBuf[i++] : 0;
		char b2 = (i < charBufLen) ? charBuf[i++] : 0;
		int int63 = 0x3F;   //  00111111 
		int int255 = 0xFF;  // 11111111
		base64Char[a++] = ALPHA_BASE[(b0 >> 2) & int63];   
		base64Char[a++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];   
		base64Char[a++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];   
		base64Char[a++] = ALPHA_BASE[b2 & int63];  
	}
	//3��һ��ת�� ��=
	switch (charBufLen % 3)
	{
	case 1:
		base64Char[--a] = '=';
	case 2:
		base64Char[--a] = '=';
	}
	//�����base64����
	base64CharLen = strlen(base64Char); 

}

void DecBase64(char* base64Char, int base64CharLen, char* outStr, int& outStrLen)
{
	int toInt[128] = { -1 };
	for (int i = 0; i < 64; i++) //��ANSI ���ҵ���Ӧ��BASE64��
	{
		int pos = ALPHA_BASE[i];
		toInt[pos] = i;
	}
	int int255 = 0xFF;  //11111111
	int index = 0;
	for (int i = 0; i < base64CharLen; i += 4)
	{
		int c0 = toInt[base64Char[i]]; // ��ȡһ���е�һ��BASE64�����
		int c1 = toInt[base64Char[i + 1]];// ��ȡһ���еڶ���BASE64�����
		outStr[index++] = (((c0 << 2) | (c1 >> 4)) & int255);  //c0��6λ  ��c1ȥ��ǰ2λ��  ��ǰ2λ  ���8bit ��ת�����ַ�  
		if (index >= base64CharLen)  //�������С����������ĳ��� ��ͼȥ��=
		{
			return;
		}
		int c2 = toInt[base64Char[i + 2]];// ��ȡһ���е�����BASE64�����
		outStr[index++] = (((c1 << 4) | (c2 >> 2)) & int255);  //c1�ĺ�6λ��c2ȥ��ǰ2λ��  ��ǰ2λ ���8bit  ת�����ַ�
		if (index >= base64CharLen)  //�������С����������ĳ���  ��ͼȥ��=
		{
			return;
		}
		int c3 = toInt[base64Char[i + 3]];// ��ȡһ���е��ĸ�BASE64�����
		outStr[index++] = (((c2 << 6) | c3) & int255); //c2�ĺ�2λ��c3�ĺ�6λ  ���8bit ת�����ַ�
	}
	//����ʹ��strlen ��Ϊ�������ݻ���'/0' Ӱ�쳤��
	outStrLen = index - 1;

}


int SaveData(CString filepath, char* Data, ULONGLONG Datalen) {
	CFile file;
	if (!file.Open(filepath, CFile::modeCreate | CFile::modeReadWrite)) {
		AfxMessageBox(_T("��ȡʧ��"));
		return InitError;
	}
	int writedlen = 1024;
	int len = 0;
	char* pdata = Data;
	while (writedlen<Datalen){
		file.Write(pdata, 1024);
		//ָ����һ������
		pdata += 1024; 
		Datalen -= 1024;
	}
	file.Write(pdata, (int)Datalen);
	file.Close();
	return Success;
}


CString FileDlg_Save(char *Data,ULONGLONG Datalen) {
	bool isOpen = FALSE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		if (SaveData(OpenFileDlg.GetPathName(), Data, Datalen) != Success) {
			AfxMessageBox(_T("д��ʧ��"));
		}
		else {
			AfxMessageBox(_T("д��ɹ�"));
		}
	}
	return OpenFileDlg.GetPathName();
}