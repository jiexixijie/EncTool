#pragma once
#include <openssl/evp.h>
#include "EnumList.h"
class C_SymmetricEnc
{
public:
	C_SymmetricEnc();
	~C_SymmetricEnc();
	virtual int Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	virtual int Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv = NULL);
	int EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv = NULL, int type = CBC);
	//virtual int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, char* iv = NULL, int type = CBC);

	//virtual int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	//加密读取文件和解密写入文件
	int EncFile(CString filename, CString outfile, char* key, char* iv = NULL, int type = CBC);
	int DecFile(CString filename, CString outfile, char* key, char* iv = NULL, int type = CBC);
};

