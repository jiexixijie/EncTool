#pragma once
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "C_SymmetricEnc.h"
class C_AES:public virtual C_SymmetricEnc
{
public:
	C_AES();
	~C_AES();
	int Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	int Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	//do_encrypt用于判断是加密还是解密 1:加密 0:解密 
	//1024字节 分块
	//再加密前需要设置KeySize 否则默认为128bit
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	int SetKeySize(int size);
private:
	int error;
	//默认为128bit
	int KeySize;
};

