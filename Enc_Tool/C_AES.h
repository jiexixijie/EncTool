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
	//再加密前需要设置KeySize 否则默认为128bit
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv = NULL, int type = CBC);
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	int SetKeySize(int size);
private:
	int error;
	//默认为128bit
	int KeySize;
};

