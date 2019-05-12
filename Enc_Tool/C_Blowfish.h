#pragma once
#include <openssl/des.h>
#include <openssl/evp.h>
#include "EnumList.h"

class C_Blowfish
{
public:
	C_Blowfish();
	~C_Blowfish();
	//PKCS padding
	//���� ���ĳ��� ���� ���ĳ��� key��Կ8�ֽ� iv-�㷨���� 8�ֽ� Ĭ��CBCģʽ
	//ֱ�Ӳ�ȡPKCS#7 ��䷽ʽ
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv = NULL, int type = CBC);
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
private:
	int error;
};

