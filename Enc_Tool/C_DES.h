#pragma once
#include <openssl/des.h>
#include <openssl/evp.h>
#include "EnumList.h"

class C_DES
{
public:
	C_DES();
	~C_DES();
	//PKCS padding
	//���� ���ĳ��� ���� ���ĳ��� key��Կ8�ֽ� iv-�㷨���� 8�ֽ� Ĭ��CBCģʽ
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv = NULL, int type = CBC);
	int DecData(char* cipher, int& cipherlen, char* msg, int& msglen, char* key, char* iv = NULL, int type = CBC);
private:
	//Ĭ��iv-�㷨����
	char* m_iv;
	int error;
};

