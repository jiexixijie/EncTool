#include "pch.h"
#include "C_DES.h"
#include <iostream>
C_DES::C_DES() {
	error = 0;
	m_iv = new char[8];
	for (int i = 0; i < 8; i++)
	{
		m_iv[i] = i+1;
	}
}

C_DES::~C_DES() {
	delete[]m_iv;
}
int C_DES::EncData(char*msg,int msglen,char*cipher, int& cipherlen,char *key,char *iv,int type) {
	error = Success;
	if (iv == NULL) {
		//设置为默认初始化向量
		iv = m_iv;
	}
	if (key == NULL || strlen(key) > EVP_MAX_KEY_LENGTH || strlen(iv) > EVP_MAX_KEY_LENGTH || strlen(iv) < 8) {
		error = KeyError;
		return error;
	}
	EVP_CIPHER_CTX ctx;//EVP算法上下文
	int ciphertmp;
	int rv;
	//初始化密码算法结构体
	EVP_CIPHER_CTX_init(&ctx);
	//设置算法和密钥 PKCS padding
	const EVP_CIPHER* ciphertype = EVP_des_cbc();
	if (type == ECB) {
		ciphertype = EVP_des_ecb();
	}
	else if (type == CBC) {
		ciphertype = EVP_des_cbc();
	}
	else if (type == CFB) {
		ciphertype = EVP_des_cfb();
	}
	else if (type == OFB) {
		ciphertype = EVP_des_ofb();
	}
	else {
		error = WrongType;
		return error;
	}
	rv = EVP_EncryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv);
	if (rv != 1)
	{
		error = InitError;
		return error;
	}
	//数据加密
	if (EVP_EncryptUpdate(&ctx, (unsigned char*)cipher, &cipherlen, (unsigned char*)msg, msglen) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	//结束数据加密，把剩余数据输出。
	if(EVP_EncryptFinal_ex(&ctx, (unsigned char*)(cipher + cipherlen), &ciphertmp) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	cipherlen += ciphertmp;
	return error;
}

int C_DES::DecData(char* cipher, int &cipherlen, char* msg, int &msglen, char* key, char* iv, int type) {
	error = Success;
	if (iv == NULL) {
		//设置为默认初始化向量
		iv = m_iv;
	}
	//iv长度至少为8
	if (key == NULL || strlen(key) > EVP_MAX_KEY_LENGTH || strlen(iv) > EVP_MAX_KEY_LENGTH || strlen(iv) < 8) {
		error = KeyError;
		return error;
	}
	EVP_CIPHER_CTX ctx;//EVP算法上下文
	int msgtmp;
	int rv;
	//初始化密码算法结构体
	EVP_CIPHER_CTX_init(&ctx);
	//设置算法和密钥 
	const EVP_CIPHER* ciphertype = EVP_des_cbc();
	if (type == ECB) {
		ciphertype = EVP_des_ecb();
	}
	else if (type == CBC) {
		ciphertype = EVP_des_cbc();
	}
	else if (type == CFB) {
		ciphertype = EVP_des_cfb();
	}
	else if (type == OFB) {
		ciphertype = EVP_des_ofb();
	}
	else {
		error = WrongType;
		return error;
	}
	rv = EVP_DecryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv);
	//数据加密
	if (EVP_DecryptUpdate(&ctx, (unsigned char*)msg, &msglen, (unsigned char*)cipher, cipherlen) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	//结束数据加密，把剩余数据输出。
	if (EVP_DecryptFinal_ex(&ctx, (unsigned char*)(msg + msglen), &msgtmp) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	msglen += msgtmp;
	return error;
}

