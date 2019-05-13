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
	//生成公私密钥对并传递给pub 和 pri
	int GenerateKey(int bits = 512, unsigned long m_e = 0x10001);
	//encpwd为加密密钥方式  
	//CertType用于判断写入密钥对的格式
	//从文件导入/出公私钥   PEM格式  pkcs#8/pkcs#1
	int WritePriKey(char* filename, CString C_Encpwd,int CertType);
	int WritePubKey(char* filename,int CertType);
	int ReadPriKey(char* filename, CString C_Decpwd, int CertType);
	int ReadPubKey(char* filename, int CertType);
	//Cstring和RSA *r相互转化
	int PriKey_to_Cstring(CString &Prikey, CString C_Encpwd, int CertType);
	int PubKey_to_Cstring(CString &Pubkey, int CertType);
	int Cstring_to_PriKey(CString Prikey, CString C_Decpwd, int CertType);
	int Cstring_to_PubKey(CString Pubkey, int CertType);
	//PriEnc, PriDec,PubEnc,PubDec,
	//需要判断明文长度 最长不能超过bit/8 -11 个字节
	//返回值如果是正 则是加密后密文长度
	int DealData(CString &C_from,CString &C_to,int type, int m_padding = RSA_PKCS1_PADDING);
	int DealData(char* from, int flen, char* to, int &tlen, int type, int m_padding = RSA_PKCS1_PADDING);
	void PrintPubKey();
	void PrintPriKey();
	//设置填充方式
	void SetPadding(int a=RSA_PKCS1_PADDING);
	//私钥签名 哈希方式:MD5, SHA1, SHA256
	//data:消息内容 sign:存放待签名的缓冲区
	//返回sign内容和长度 需要先ReadPriKey() 或 GenerateKey()
	int Pri_Sign(char* data, int datalen, char* sign, int &signlen, int mdtype = Type_MD5);
	int Pri_Sign(CString data, CString &sign, int mdtype = Type_MD5);
	//公钥验证  
	//data:消息内容 sign:签名值 进行验证
	int Pub_Verfiy(char* data, int datalen, char* sign, unsigned int signlen, int mdtype = Type_MD5);
	int Pub_Verfiy(CString data, CString sign, int mdtype = Type_MD5);
private:
	RSA *Pub;
	RSA* Pri;
	int error;
	int padding;//默认为RSA_PKCS1_PADDING
	int bits; //默认为512
};

