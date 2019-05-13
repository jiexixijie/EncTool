#pragma once
#include <iostream>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <string.h>
#include <afx.h>
#include "EnumList.h"
//enum RSA_Error {
//	GenerateError = -10, WriteError, ReadError, Enc_Dec_Fail, WrongType, DataBigError, Success = 0,
//};


//{1,3,5}

class C_RSA
{
public:
	C_RSA();
	~C_RSA();
	//���ɹ�˽��Կ�Բ����ݸ�pub �� pri
	int GenerateKey(int bits = 512, unsigned long m_e = 0x10001);
	//encpwdΪ������Կ��ʽ  
	//CertType�����ж�д����Կ�Եĸ�ʽ
	//���ļ�����/����˽Կ   PEM��ʽ  pkcs#8/pkcs#1
	int WritePriKey(char* filename, CString C_Encpwd,int CertType);
	int WritePubKey(char* filename,int CertType);
	int ReadPriKey(char* filename, CString C_Decpwd, int CertType);
	int ReadPubKey(char* filename, int CertType);
	//Cstring��RSA *r�໥ת��
	int PriKey_to_Cstring(CString &Prikey, CString C_Encpwd, int CertType);
	int PubKey_to_Cstring(CString &Pubkey, int CertType);
	int Cstring_to_PriKey(CString Prikey, CString C_Decpwd, int CertType);
	int Cstring_to_PubKey(CString Pubkey, int CertType);
	//PriEnc, PriDec,PubEnc,PubDec,
	//��Ҫ�ж����ĳ��� ����ܳ���bit/8 -11 ���ֽ�
	//����ֵ������� ���Ǽ��ܺ����ĳ���
	int DealData(CString &C_from,CString &C_to,int type, int m_padding = RSA_PKCS1_PADDING);
	int DealData(char* from, int flen, char* to, int &tlen, int type, int m_padding = RSA_PKCS1_PADDING);
	void PrintPubKey();
	void PrintPriKey();
	//������䷽ʽ
	void SetPadding(int a=RSA_PKCS1_PADDING);
	//˽Կǩ�� ��ϣ��ʽ:MD5, SHA1, SHA256
	//data:��Ϣ���� sign:��Ŵ�ǩ���Ļ�����
	//����sign���ݺͳ��� ��Ҫ��ReadPriKey() �� GenerateKey()
	int Pri_Sign(char* data, int datalen, char* sign, int &signlen, int mdtype = Type_MD5);
	int Pri_Sign(CString data, CString &sign, int mdtype = Type_MD5);
	//��Կ��֤  
	//data:��Ϣ���� sign:ǩ��ֵ ������֤
	int Pub_Verfiy(char* data, int datalen, char* sign, unsigned int signlen, int mdtype = Type_MD5);
	int Pub_Verfiy(CString data, CString sign, int mdtype = Type_MD5);
private:
	RSA *Pub;
	RSA* Pri;
	int error;
	int padding;//Ĭ��ΪRSA_PKCS1_PADDING
	int bits; //Ĭ��Ϊ512
};

