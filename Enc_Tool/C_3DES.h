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
	//���� ���ĳ��� ���� ���ĳ��� key��Կ24�ֽ� iv-�㷨���� 8�ֽ� Ĭ��CBCģʽ
	//ֱ�Ӳ�ȡPKCS#7 ��䷽ʽ
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	//���ܶ�ȡ�ļ��ͽ���д���ļ�
};


