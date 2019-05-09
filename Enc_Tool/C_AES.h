#pragma once
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "EnumList.h"
class C_AES
{
public:
	C_AES();
	~C_AES();
	//do_encrypt用于判断是加密还是解密 1:加密 0:解密 
	//1024字节 分块
	int DealData(char* inbuf, int inlen, char* outbuf, int& outlen, char* key, int do_encrypt, char* iv = NULL, int type = CBC);
	int SetBlockSize(int size);
private:
	int error;
	//至少16字节
	char* m_iv;
	//默认为128bit
	int BlockSize;
};

