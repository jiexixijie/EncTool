#include "pch.h"
#include "C_AES.h"
#include <iostream>

C_AES::C_AES(){
	error = Success;
	m_iv = new char[16];
	for (int i = 0; i < 16; i++)
	{
		m_iv[i] = i+1;
	}
	BlockSize = 128;
}

C_AES::~C_AES()
{
	delete[]m_iv;
}

int C_AES::SetBlockSize(int size) {
	error = Success;
	if (size != 128 && size != 192 && size != 256) {
		error = WrongBlockSize;
	}
	return error;
}

int C_AES::DealData(char* inbuf, int inlen, char* outbuf, int& outlen, char* key,int do_encrypt, char* iv, int type) {
	error = Success;
	if (iv == NULL) {
		//设置为默认初始化向量
		iv = m_iv;
	}
	//iv长度至少为16
	if (key == NULL || strlen(key) > EVP_MAX_KEY_LENGTH || strlen(iv) > EVP_MAX_KEY_LENGTH || strlen(iv) < 16) {
		error = KeyError;
		return error;
	}
	//EVP算法上下文
    EVP_CIPHER_CTX ctx;
	int ciphertmp;
	int rv;
	//初始化密码算法结构体
	EVP_CIPHER_CTX_init(&ctx);
	//设置算法和密钥 PKCS padding
	const EVP_CIPHER* ciphertype = EVP_des_cbc();
	if (type == ECB ) {
		if (BlockSize == 192) {
			ciphertype = EVP_aes_192_ecb();
		}
		else if (BlockSize == 256) {
			ciphertype = EVP_aes_256_ecb();
		}
		else {
			ciphertype = EVP_aes_128_ecb();
		}
	}
	else if (type == CBC) {
		if (BlockSize == 192) {
			ciphertype = EVP_aes_192_cbc();
		}
		else if (BlockSize == 256) {
			ciphertype = EVP_aes_256_cbc();
		}
		else {
			ciphertype = EVP_aes_128_cbc();
		}
	}
	else if (type == CFB) {
		if (BlockSize == 192) {
			ciphertype = EVP_aes_192_cfb();
		}
		else if (BlockSize == 256) {
			ciphertype = EVP_aes_256_cfb();
		}
		else {
			ciphertype = EVP_aes_128_cfb();
		};
	}
	else if (type == OFB) {
		if (BlockSize == 192) {
			ciphertype = EVP_aes_192_ofb();
		}
		else if (BlockSize == 256) {
			ciphertype = EVP_aes_256_ofb();
		}
		else {
			ciphertype = EVP_aes_128_ofb();
		};
	}
	else {
		error = WrongType;
		goto AES_Deal_end;
	}
	rv = EVP_CipherInit_ex(&ctx, ciphertype, NULL, NULL, NULL,do_encrypt);
	OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
	OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);
	EVP_CipherInit_ex(&ctx, NULL, NULL, (unsigned char*)key, (unsigned char*)iv, do_encrypt);
	if (rv != 1){
		error = InitError;
		
	}
	//数据加解密
	if (EVP_CipherUpdate(&ctx, (unsigned char*)outbuf, &outlen, (unsigned char*)inbuf, inlen) != 1) {
		error = Enc_Dec_Fail;
		goto AES_Deal_end;
	}
	//结束数据加密，把剩余数据输出。
	if (EVP_CipherFinal_ex(&ctx, (unsigned char*)(outbuf + outlen), &ciphertmp) != 1) {
		error = Enc_Dec_Fail;
		goto AES_Deal_end;
	}
	outlen += ciphertmp;
AES_Deal_end:
	return error;
}
