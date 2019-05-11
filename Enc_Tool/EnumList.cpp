#include "pch.h"
#include <afx.h>
#include <string>
#include "EnumList.h"

//BASE64  64个字符
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
	//3个一组转换 补=
	switch (charBufLen % 3)
	{
	case 1:
		base64Char[--a] = '=';
	case 2:
		base64Char[--a] = '=';
	}
	//输出的base64长度
	base64CharLen = strlen(base64Char); 

}

void DecBase64(char* base64Char, int base64CharLen, char* outStr, int& outStrLen)
{
	int toInt[128] = { -1 };
	for (int i = 0; i < 64; i++) //将ANSI 码找到对应的BASE64码
	{
		int pos = ALPHA_BASE[i];
		toInt[pos] = i;
	}
	int int255 = 0xFF;  //11111111
	int index = 0;
	for (int i = 0; i < base64CharLen; i += 4)
	{
		int c0 = toInt[base64Char[i]]; // 获取一组中第一个BASE64编码号
		int c1 = toInt[base64Char[i + 1]];// 获取一组中第二个BASE64编码号
		outStr[index++] = (((c0 << 2) | (c1 >> 4)) & int255);  //c0后6位  和c1去完前2位的  的前2位  组成8bit 再转换成字符  
		int c2 = toInt[base64Char[i + 2]];// 获取一组中第三个BASE64编码号
		outStr[index++] = (((c1 << 4) | (c2 >> 2)) & int255);  //c1的后6位和c2去完前2位的  的前2位 组成8bit  转换成字符
		//若有两个'=' 提前结束
		if (base64Char[i + 2] == '=') {
			break;
		}
		int c3 = toInt[base64Char[i + 3]];// 获取一组中第四个BASE64编码号
		outStr[index++] = (((c2 << 6) | c3) & int255); //c2的后2位和c3的后6位  组成8bit 转换成字符
	}
	//不能使用strlen 因为可能数据会有'/0' 影响长度
	outStrLen = index - 1;

}


int SaveData(CString filepath, char* Data, ULONGLONG Datalen) {
	CFile file;
	if (!file.Open(filepath, CFile::modeCreate | CFile::modeReadWrite)) {
		AfxMessageBox(_T("读取失败"));
		return InitError;
	}
	int writedlen = 1024;
	int len = 0;
	char* pdata = Data;
	while (writedlen<Datalen){
		file.Write(pdata, 1024);
		//指向下一个数据
		pdata += 1024; 
		Datalen -= 1024;
	}
	file.Write(pdata, (int)Datalen);
	file.Close();
	return Success;
}


CString FileDlg_Save(char *Data,ULONGLONG Datalen) {
	CString FilePath = FileDlg_GetSavePath();
	if (!FilePath.IsEmpty()) {
		if (SaveData(FilePath, Data, Datalen) != Success) {
			AfxMessageBox(_T("写入失败"));
		}
		else {
			AfxMessageBox(_T("写入成功"));
		}
	}
	return FilePath;
}

CString FileDlg_GetSavePath() {
	bool isOpen = FALSE;
	CString filename;
	CString filter = _T("All Files|*||");
	CFileDialog OpenFileDlg(isOpen, NULL, filename, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL);
	if (OpenFileDlg.DoModal() == IDOK) {
		return OpenFileDlg.GetPathName();
	}
	else {
		return NULL;
	}
}



//字符串换行符转换
CString TransData(CString A, int type) {
	//换行符转换 0x0A转0x0A0x0D
	int index = 0, start = 0;
	CString temp;
	//0:windows 转 unix 1 去\r : unix转windows \n 变 \r\n
	if (type == 0) {
		while (1)
		{
			index = A.Find(0x0A, index);
			if (index < 0) {
				//加上剩余部分
				int a = A.GetLength();
				//CString bbc = A.Mid(start, A.GetLength() - 1);
				temp.Format(TEXT("%s%s"), temp, A.Mid(start, A.GetLength()));
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
	return temp.IsEmpty() ? A : temp;
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
	return wlen == flen;
}


void Get_Key_Vi_c(CString key, char* key_c, CString iv, char* iv_c) {
	int keylen = WideCharToMultiByte(CP_ACP, 0, key, key.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, key, key.GetLength(), key_c, keylen, NULL, NULL);
	int ivlen = WideCharToMultiByte(CP_ACP, 0, iv, iv.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, iv, iv.GetLength(), iv_c, ivlen, NULL, NULL);
}