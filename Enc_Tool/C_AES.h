#pragma once
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "EnumList.h"
class C_AES
{
public:
	C_AES();
	~C_AES();
	//do_encrypt�����ж��Ǽ��ܻ��ǽ��� 1:���� 0:���� 
	//1024�ֽ� �ֿ�
	int DealData(char* inbuf, int inlen, char* outbuf, int& outlen, char* key, int do_encrypt, char* iv = NULL, int type = CBC);
	int SetBlockSize(int size);
private:
	int error;
	//����16�ֽ�
	char* m_iv;
	//Ĭ��Ϊ128bit
	int BlockSize;
};

