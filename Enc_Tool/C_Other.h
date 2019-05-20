#pragma once


struct EncFileHead{
	char Flag[8];
	char filename[48];		//��ż���ǰ�ļ���
	char fileType[8];		//�ļ���ʽ
	int EncType;        //����ļ����ܵķ�ʽ
	char EncKeyData[256];   //����2048bitRSA��Կ���ܺ����Կ
	int keylen;             //��RSA����ǰ ��Կ����
	ULONGLONG OriginalFileSize;
};

//AES�����ļ�
int EncFile_AES(CString FilePath);
int EncFile_DES();
int EncFile_RC4();
void GenRandomKey(BYTE* key, int len);
CString GetBrowseFolderPath(CWnd* pWnd);

