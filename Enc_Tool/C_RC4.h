#pragma once
#include <openssl/des.h>
#include <openssl/evp.h>
#include "EnumList.h"

class C_RC4
{
public:
	C_RC4();
	~C_RC4();
	//PKCS padding
	//���� ���ĳ��� ���� ���ĳ��� key��Կ8�ֽ� iv-�㷨���� 8�ֽ� Ĭ��CBCģʽ
	//ֱ�Ӳ�ȡPKCS#7 ��䷽ʽ
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, int keylen = 16);
	int EncData(CString msg, CString& cipher, char* key, int keylen = 16);
	int DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, int keylen = 16);
	int DecData(CString cipher, CString& msg, char* key_c, int keylen = 16);
private:
	int error;
};

