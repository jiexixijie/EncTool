#pragma once
#include <openssl/evp.h>
#include "EnumList.h"
class C_Hash
{
public:
	C_Hash();
	~C_Hash();
	int Hash(char* msg, int msglen, char* cipher, int& cipherlen,  int type = Type_MD5);
	int Hash(CString msg, CString &cipher, int type = Type_MD5);
};

