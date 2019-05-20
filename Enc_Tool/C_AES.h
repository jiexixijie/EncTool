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
	//do_encrypt�����ж��Ǽ��ܻ��ǽ��� 1:���� 0:���� 
	//1024�ֽ� �ֿ�
	//�ټ���ǰ��Ҫ����KeySize ����Ĭ��Ϊ128bit
	int EncData(CString msg, CString& cipher, char* key, char* iv = NULL, int type = CBC);
	int DecData(CString cipher, CString& msg, char* key_c, char* iv_c = NULL, int type = CBC);
	int SetKeySize(int size);
private:
	int error;
	//Ĭ��Ϊ128bit
	int KeySize;
};

