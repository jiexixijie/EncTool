#pragma once
#include <openssl/des.h>
#include <openssl/evp.h>
#include "C_SymmetricEnc.h"
class C_3DES:public virtual C_SymmetricEnc
{
public:
	C_3DES();
	~C_3DES();
	int Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	int Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	//PKCS padding
	//明文 明文长度 密文 密文长度 key密钥24字节 iv-算法向量 8字节 默认CBC模式
	//直接采取PKCS#7 填充方式
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	//加密读取文件和解密写入文件
};


