#pragma once


struct EncFileHead{
	char Flag[8];
	char filename[48];		//存放加密前文件名
	char fileType[8];		//文件格式
	int EncType;        //存放文件加密的方式
	char EncKeyData[256];   //用于2048bitRSA公钥加密后的密钥
	int keylen;             //被RSA加密前 密钥长度
	ULONGLONG OriginalFileSize;
};

//AES加密文件
int EncFile_AES(CString FilePath);
int EncFile_DES();
int EncFile_RC4();
void GenRandomKey(BYTE* key, int len);
CString GetBrowseFolderPath(CWnd* pWnd);

