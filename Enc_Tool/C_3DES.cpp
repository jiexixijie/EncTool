#include "pch.h"
#include "C_3DES.h"
#include <iostream>

C_3DES::C_3DES() {
}

C_3DES::~C_3DES() {
}

int C_3DES::Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv) {
	EVP_CIPHER_CTX_init(&ctx);
	const EVP_CIPHER* ciphertype;
	switch (type)
	{
	case ECB:
		ciphertype = EVP_des_ede3_ecb();
		break;
	case CBC:
		ciphertype = EVP_des_ede3_cbc();
		break;
	case CFB:
		ciphertype = EVP_des_ede3_cfb();
		break;
	case OFB:
		ciphertype = EVP_des_ede3_ofb();
		break;
	default:
		ciphertype = EVP_des_ede3_cbc();
		break;
	}
	if (EVP_EncryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv) != 1) {
		return InitError;
	}
	else {
		return Success;
	}
}

int C_3DES::Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv) {
	EVP_CIPHER_CTX_init(&ctx);
	const EVP_CIPHER* ciphertype;
	switch (type)
	{
	case ECB:
		ciphertype = EVP_des_ede3_ecb();
		break;
	case CBC:
		ciphertype = EVP_des_ede3_cbc();
		break;
	case CFB:
		ciphertype = EVP_des_ede3_cfb();
		break;
	case OFB:
		ciphertype = EVP_des_ede3_ofb();
		break;
	default:
		ciphertype = EVP_des_ede3_cbc();
		break;
	}
	if (EVP_DecryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv) != 1) {
		return InitError;
	}
	else {
		return Success;
	}
}


int C_3DES::EncData(CString msg, CString& cipher, char* key_c, char* iv_c, int type) {
	int msglen = WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), NULL, 0, NULL, NULL);
	char* msg_c = new char[msglen + 1];
	WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), msg_c, msglen, NULL, NULL);
	msg_c[msglen] = '\0';
	int cipherlen = 0;
	int cipherbaselen = 0;
	//8字节一组 加8防止有填充长度增加
	char* cipher_c = new char[msglen + 8];
	char* cipher_base = new char[msglen * 2 + 12]; //  /3*4+1
	memset(cipher_base, 0, msglen * 2 + 12);
	memset(cipher_c, 0, msglen + 8);
	if (C_SymmetricEnc::EncData(msg_c, msglen, cipher_c, cipherlen, key_c, iv_c, type) != Success) {
		delete[]msg_c;
		delete[]cipher_c;
		delete[]cipher_base;
		return  Enc_Dec_Fail;
	}
	//加密成功对密文进行BASE64加密
	EncBase64(cipher_c, cipherlen, cipher_base, cipherbaselen);
	cipher = cipher_base;
	delete[]msg_c;
	delete[]cipher_c;
	delete[]cipher_base;
	return Success;
}


int C_3DES::DecData(CString cipher, CString& msg, char* key_c, char* iv_c, int type) {
	int a = cipher.GetLength();
	int cipherlen = WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), NULL, 0, NULL, NULL);
	char* cipher_c = new char[cipherlen + 1];
	WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), cipher_c, cipherlen, NULL, NULL);
	cipher_c[cipherlen] = '\0';
	//要先对密文进行BASE64解密
	char* cipher_Decbase = new char[cipherlen + 1]; //1024/3*4+1
	memset(cipher_Decbase, 0, cipherlen + 1);
	DecBase64(cipher_c, cipherlen, cipher_Decbase, cipherlen);
	//8字节一组 加8防止有填充长度增加
	char* msg_c = new char[cipherlen + 1];
	int msglen = 0;
	memset(msg_c, 0, cipherlen + 1);
	if (C_SymmetricEnc::DecData(cipher_Decbase, cipherlen, msg_c, msglen, key_c, iv_c, type) != Success) {
		delete[]cipher_c;
		delete[]cipher_Decbase;
		delete[]msg_c;
		return  Enc_Dec_Fail;
	}
	//进行截断padding
	msg_c[msglen] = '\0';

	msg = msg_c;
	delete[]cipher_c;
	delete[]cipher_Decbase;
	delete[]msg_c;
	return Success;
}

