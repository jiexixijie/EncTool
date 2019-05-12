#include "pch.h"
#include "C_RC4.h"
#include <iostream>
#include <openssl/rc4.h>
C_RC4::C_RC4() {
	error = 0;
}

C_RC4::~C_RC4() {
}
int C_RC4::EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, int keylen) {
	//初始化密码算法结构体
	RC4_KEY RC4KEY;
	RC4_set_key(&RC4KEY, keylen, (unsigned char*)key);
	RC4(&RC4KEY, msglen, (unsigned char*)msg,(unsigned char*)cipher);
	cipherlen = msglen;
	return Success;
}

int C_RC4::DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, int keylen) {
	RC4_KEY RC4KEY;
	RC4_set_key(&RC4KEY, keylen, (unsigned char*)key);
	RC4(&RC4KEY, cipherlen, (unsigned char*)cipher, (unsigned char*)msg);
	msglen = cipherlen;
	return Success;
}

int C_RC4::EncData(CString msg, CString& cipher, char* key_c, int keylen ) {
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
	if (EncData(msg_c, msglen, cipher_c, cipherlen, key_c, keylen) != Success) {
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


int C_RC4::DecData(CString cipher, CString& msg, char* key_c, int keylen) {
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
	if (DecData(cipher_Decbase, cipherlen, msg_c, msglen, key_c, keylen) != Success) {
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

