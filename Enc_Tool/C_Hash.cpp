#include "pch.h"
#include "C_Hash.h"


C_Hash::C_Hash()
{
}


C_Hash::~C_Hash()
{
}

int C_Hash::Hash(char* msg, int msglen, char* cipher, int& cipherlen, int type ) {
	EVP_MD_CTX mctx;
	EVP_MD_CTX_init(&mctx);
	const EVP_MD *md;
	if (type == Type_MD5) {
		md = EVP_md5();
	}
	else if (type == Type_SHA1) {
		md = EVP_sha1();
	}
	else {
		md = EVP_sha256();
	}
	if (EVP_DigestInit_ex(&mctx, md, NULL) != 1) {
		return InitError;
	}
	if (EVP_DigestUpdate(&mctx, msg, msglen) != 1) {
		return EVPHashError;
	}
	if(EVP_DigestFinal(&mctx,(unsigned char*)cipher,(unsigned int*)&cipherlen)!=1) {
		return EVPHashError;
	}
	return Success;
}

int C_Hash::Hash(CString msg, CString& cipher, int type ) {
	int msglen = WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), NULL, 0, NULL, NULL);
	char* msg_c = new char[msglen + 1];
	WideCharToMultiByte(CP_ACP, 0, msg, msg.GetLength(), msg_c, msglen, NULL, NULL);
	msg_c[msglen] = '\0';
	//MD5输出128bit
	//SHA1输出160bit
	//SHA256输出256bit
	char cipher_c[256];
	memset(cipher_c, 0, 256);
	int cipherlen = 0;
	if (Hash(msg_c, msglen, cipher_c, cipherlen, type) != Success) {
		delete[]msg_c;
		return EVPHashError;
	}
	//哈希输出 进行BASE64
	cipher = Data_To_HexCString((BYTE *)cipher_c,cipherlen);
	delete[]msg_c;
	return Success;
}