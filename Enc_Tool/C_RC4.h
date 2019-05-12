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
	//明文 明文长度 密文 密文长度 key密钥8字节 iv-算法向量 8字节 默认CBC模式
	//直接采取PKCS#7 填充方式
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, int keylen = 16);
	int EncData(CString msg, CString& cipher, char* key, int keylen = 16);
	int DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, int keylen = 16);
	int DecData(CString cipher, CString& msg, char* key_c, int keylen = 16);
private:
	int error;
};

