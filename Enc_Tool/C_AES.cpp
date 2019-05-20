#include "pch.h"
#include "C_AES.h"
#include <iostream>

C_AES::C_AES(){
	error = Success;
	KeySize = 128;
}

C_AES::~C_AES()
{
}

int C_AES::Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv) {
	EVP_CIPHER_CTX_init(&ctx);
	const EVP_CIPHER* ciphertype;
	if (type == ECB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_ecb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_ecb();
		}
		else {
			ciphertype = EVP_aes_128_ecb();
		}
	}
	else if (type == CBC) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_cbc();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_cbc();
		}
		else {
			ciphertype = EVP_aes_128_cbc();
		}
	}
	else if (type == CFB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_cfb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_cfb();
		}
		else {
			ciphertype = EVP_aes_128_cfb();
		};
	}
	else if (type == OFB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_ofb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_ofb();
		}
		else {
			ciphertype = EVP_aes_128_ofb();
		};
	}
	else {
		return InitError;
	}
	if (EVP_EncryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv) != 1) {
		return InitError;
	}
	else {
		return Success;
	}
}

int C_AES::Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv) {
	EVP_CIPHER_CTX_init(&ctx);
	const EVP_CIPHER* ciphertype;
	if (type == ECB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_ecb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_ecb();
		}
		else {
			ciphertype = EVP_aes_128_ecb();
		}
	}
	else if (type == CBC) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_cbc();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_cbc();
		}
		else {
			ciphertype = EVP_aes_128_cbc();
		}
	}
	else if (type == CFB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_cfb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_cfb();
		}
		else {
			ciphertype = EVP_aes_128_cfb();
		};
	}
	else if (type == OFB) {
		if (KeySize == 192) {
			ciphertype = EVP_aes_192_ofb();
		}
		else if (KeySize == 256) {
			ciphertype = EVP_aes_256_ofb();
		}
		else {
			ciphertype = EVP_aes_128_ofb();
		};
	}
	else {
		return InitError;
	}
	if (EVP_DecryptInit_ex(&ctx, ciphertype, NULL, (unsigned char*)key, (unsigned char*)iv) != 1) {
		return InitError;
	}
	else {
		return Success;
	}
}


int C_AES::SetKeySize(int size) {
	error = Success;
	if (size != 128 && size != 192 && size != 256) {
		error = WrongKeySize;
	}
	else {
		KeySize = size;
	}
	return error;
}


int C_AES::EncData(CString msg, CString& cipher, char* key_c, char* iv_c, int type) {
	int msglen = WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), NULL, 0, NULL, NULL);
	char* msg_c = new char[msglen + 1];
	WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), msg_c, msglen, NULL, NULL);
	msg_c[msglen] = '\0';
	int cipherlen = 0;
	int cipherbaselen = 0;
	//16�ֽ�һ�� ��16��ֹ����䳤������
	char* cipher_c = new char[msglen + 16];
	char* cipher_base = new char[msglen * 2 + 25]; //  /3*4+1
	memset(cipher_base, 0, msglen * 2 + 25);
	memset(cipher_c, 0, msglen + 16);
	if (C_SymmetricEnc::EncData(msg_c, msglen, cipher_c, cipherlen, key_c, iv_c, type) != Success) {
		delete[]msg_c;
		delete[]cipher_c;
		delete[]cipher_base;
		return  Enc_Dec_Fail;
	}
	//���ܳɹ������Ľ���BASE64����
	EncBase64(cipher_c, cipherlen, cipher_base, cipherbaselen);
	cipher = cipher_base;
	delete[]msg_c;
	delete[]cipher_c;
	delete[]cipher_base;
	return Success;
}

int C_AES::DecData(CString cipher, CString& msg, char* key_c, char* iv_c, int type) {
	int cipherlen = WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), NULL, 0, NULL, NULL);
	char* cipher_c = new char[cipherlen + 1];
	WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), cipher_c, cipherlen, NULL, NULL);
	cipher_c[cipherlen] = '\0';
	//Ҫ�ȶ����Ľ���BASE64����
	char* cipher_Decbase = new char[cipherlen + 1]; //1024/3*4+1
	memset(cipher_Decbase, 0, cipherlen + 1);
	DecBase64(cipher_c, cipherlen, cipher_Decbase, cipherlen);
	//8�ֽ�һ�� ��8��ֹ����䳤������
	char* msg_c = new char[cipherlen + 1];
	int msglen = 0;
	memset(msg_c, 0, cipherlen + 1);
	if (C_SymmetricEnc::DecData(cipher_Decbase, cipherlen, msg_c, msglen, key_c, iv_c, type) != Success) {
		delete[]cipher_c;
		delete[]cipher_Decbase;
		delete[]msg_c;
		return  Enc_Dec_Fail;
	}
	//���нض�padding
	msg_c[msglen] = '\0';

	msg = msg_c;
	delete[]cipher_c;
	delete[]cipher_Decbase;
	delete[]msg_c;
	return Success;
}
