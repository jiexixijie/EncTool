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
		//����ΪĬ�ϳ�ʼ������
		iv = m_iv;
	}
	EVP_CIPHER_CTX ctx;//EVP�㷨������
	int ciphertmp;
	int rv;
	//��ʼ�������㷨�ṹ��
	EVP_CIPHER_CTX_init(&ctx);
	//�����㷨����Կ PKCS padding
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
	//���ݼ���
	if (EVP_EncryptUpdate(&ctx, (unsigned char*)cipher, &cipherlen, (unsigned char*)msg, msglen) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	//�������ݼ��ܣ���ʣ�����������
	if(EVP_EncryptFinal_ex(&ctx, (unsigned char*)(cipher + cipherlen), &ciphertmp) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	cipherlen += ciphertmp;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return error;
}

int C_DES::DecData(char* cipher, int cipherlen, char* msg, int &msglen, char* key, char* iv, int type) {
	error = Success;
	if (iv == NULL) {
		//����ΪĬ�ϳ�ʼ������
		iv = m_iv;
	}
	EVP_CIPHER_CTX ctx;//EVP�㷨������
	int msgtmp;
	int rv;
	//��ʼ�������㷨�ṹ��
	EVP_CIPHER_CTX_init(&ctx);
	//�����㷨����Կ 
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
	//���ݽ���
	if (EVP_DecryptUpdate(&ctx, (unsigned char*)msg, &msglen, (unsigned char*)cipher, cipherlen) != 1) {
		error = Enc_Dec_Fail;
		return error;
	}
	//�������ݼ��ܣ���ʣ�����������
	if (EVP_DecryptFinal_ex(&ctx, (unsigned char*)(msg + msglen), &msgtmp) != 1) {
			error = Enc_Dec_Fail;
			return error;
	}
	msglen += msgtmp;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return error;
}

int C_DES::EncData(CString msg, CString& cipher, char* key_c, char* iv_c, int type) {
	int msglen = WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), NULL, 0, NULL, NULL);
	char* msg_c = new char[msglen + 1];
	WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), msg_c, msglen, NULL, NULL);
	msg_c[msglen] = '\0';
	int cipherlen = 0;
	int cipherbaselen = 0;
	//8�ֽ�һ�� ��8��ֹ����䳤������
	char* cipher_c = new char[msglen + 8];
	char* cipher_base = new char[msglen * 2 + 12]; //  /3*4+1
	memset(cipher_base, 0, msglen * 2 + 12);
	memset(cipher_c, 0, msglen + 8);
	if (EncData(msg_c, msglen, cipher_c, cipherlen, key_c, iv_c, type) != Success) {
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


int C_DES::DecData(CString cipher, CString& msg, char* key_c, char* iv_c, int type) {
	int a = cipher.GetLength();
	int cipherlen = WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), NULL, 0, NULL, NULL);
	char* cipher_c = new char[cipherlen + 1];
	WideCharToMultiByte(CP_ACP, 0, cipher, cipher.GetLength(), cipher_c, cipherlen, NULL, NULL);
	cipher_c[cipherlen] = '\0';
	//Ҫ�ȶ����Ľ���BASE64����
	char *cipher_Decbase=new char[cipherlen + 1]; //1024/3*4+1
	memset(cipher_Decbase, 0, cipherlen + 1);
	DecBase64(cipher_c, cipherlen, cipher_Decbase, cipherlen);
	//8�ֽ�һ�� ��8��ֹ����䳤������
	char* msg_c = new char[cipherlen + 1];
	int msglen = 0;
	memset(msg_c, 0, cipherlen + 1);
	if (DecData(cipher_Decbase, cipherlen, msg_c, msglen, key_c, iv_c, type) != Success) {
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


void Get_Key_Vi_c(CString key, char* key_c, CString iv, char* iv_c) {
	//key��iv��Ϊ8�ֽ�
	int keylen = WideCharToMultiByte(CP_ACP, 0, key, key.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, key, key.GetLength(), key_c, keylen, NULL, NULL);
	if (iv.IsEmpty()) {
		iv_c = NULL;
		return;
	}
	int ivlen = WideCharToMultiByte(CP_ACP, 0, iv, iv.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, iv, iv.GetLength(), iv_c, keylen, NULL, NULL);
}