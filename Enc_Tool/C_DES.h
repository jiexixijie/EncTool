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
	//明文 明文长度 密文 密文长度 key密钥8字节 iv-算法向量 8字节 默认CBC模式
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv = NULL, int type = CBC);
	int DecData(char* cipher, int& cipherlen, char* msg, int& msglen, char* key, char* iv = NULL, int type = CBC);
private:
	//默认iv-算法向量
	char* m_iv;
	int error;
};

