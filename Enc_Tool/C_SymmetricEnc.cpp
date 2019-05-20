#include "pch.h"
#include "C_SymmetricEnc.h"


C_SymmetricEnc::C_SymmetricEnc()
{

}


C_SymmetricEnc::~C_SymmetricEnc()
{

}

int C_SymmetricEnc::Init_Enc_evp_ctx(EVP_CIPHER_CTX& ctx,int type, char* key, char* iv) {
	/*EVP_CIPHER_CTX_init(&ctx);
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
	}*/
	return InitError;
}


int C_SymmetricEnc::Init_Dec_evp_ctx(EVP_CIPHER_CTX& ctx, int type, char* key, char* iv) {
	/*EVP_CIPHER_CTX_init(&ctx);
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
	}*/
	return InitError;
}

int C_SymmetricEnc::EncData(char* msg, int msglen, char* cipher, int& cipherlen, char* key, char* iv, int type) {
	EVP_CIPHER_CTX ctx;//EVP算法上下文
	int result = Init_Enc_evp_ctx(ctx, type, key, iv);
	if (result != Success) {
		return result;
	}
	int ciphertmp;
	//数据加密
	if (EVP_EncryptUpdate(&ctx, (unsigned char*)cipher, &cipherlen, (unsigned char*)msg, msglen) != 1) {
		return Enc_Dec_Fail;
	}
	//结束数据加密，把剩余数据输出。
	if (EVP_EncryptFinal_ex(&ctx, (unsigned char*)(cipher + cipherlen), &ciphertmp) != 1) {
		return Enc_Dec_Fail;
	}
	cipherlen += ciphertmp;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return Success;
}

int C_SymmetricEnc::DecData(char* cipher, int cipherlen, char* msg, int& msglen, char* key, char* iv, int type) {
	EVP_CIPHER_CTX ctx;//EVP算法上下文
	int result = Init_Dec_evp_ctx(ctx, type, key, iv);
	if (result != Success) {
		return result;
	}
	int msgtmp;
	//数据解密
	if (EVP_DecryptUpdate(&ctx, (unsigned char*)msg, &msglen, (unsigned char*)cipher, cipherlen) != 1) {
		return Enc_Dec_Fail;
	}
	//结束数据加密，把剩余数据输出。
	if (EVP_DecryptFinal_ex(&ctx, (unsigned char*)(msg + msglen), &msgtmp) != 1) {
		return Enc_Dec_Fail;
	}
	msglen += msgtmp;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return Success;
}

int C_SymmetricEnc::EncFile(CString filename, CString outfile, char* key, char* iv, int type) {
	CFile fileR, fileW;
	if (fileR.Open(filename, CFile::modeRead) == 0) {
		return ReadError;
	}
	if (fileW.Open(outfile, CFile::modeCreate | CFile::modeReadWrite) == 0) {
		fileR.Close();
		return WriteError;
	}

	EVP_CIPHER_CTX ctx;//EVP算法上下文
	//设置算法和密钥 PKCS padding
	int result = Init_Enc_evp_ctx(ctx, type, key, iv);
	if (result != Success) {
		return result;
	}

	char filedata[1024], EncData[1024 + EVP_MAX_BLOCK_LENGTH];
	//每1024字节一个循环 解密文件 直到最后一个使用finally
	int Writesize = 0;
	while (TRUE) {
		//全部读取完毕
		memset(filedata, 0, 1024);
		memset(EncData, 0, 1024 + EVP_MAX_BLOCK_LENGTH);
		int readsize = fileR.Read(filedata, 1024);
		if (readsize <= 0) {
			break;
		}
		//数据加密
		if (EVP_EncryptUpdate(&ctx, (unsigned char*)EncData, &Writesize, (unsigned char*)filedata, readsize) != 1) {
			fileR.Close();
			fileW.Close();
			EVP_CIPHER_CTX_cleanup(&ctx);
			CFile::Remove(outfile);
			return Enc_Dec_Fail;
		}
		//写入部分密文
		fileW.Write(EncData, Writesize);
	}
	if (EVP_EncryptFinal_ex(&ctx, (unsigned char*)EncData, &Writesize) != 1) {
		fileR.Close();
		fileW.Close();
		CFile::Remove(outfile);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return Enc_Dec_Fail;
	}
	fileW.Write(EncData, Writesize);
	fileR.Close();
	fileW.Close();
	EVP_CIPHER_CTX_cleanup(&ctx);
	return Success;
}


int C_SymmetricEnc::DecFile(CString filename, CString outfile, char* key, char* iv, int type) {
	CFile fileR, fileW;
	if (fileR.Open(filename, CFile::modeRead) == 0) {
		return ReadError;
	}
	if (fileW.Open(outfile, CFile::modeCreate | CFile::modeReadWrite) == 0) {
		fileR.Close();
		return WriteError;
	}

	EVP_CIPHER_CTX ctx;//EVP算法上下文
	int result = Init_Dec_evp_ctx(ctx, type, key, iv);
	if (result != Success) {
		return result;
	}

	char filedata[1024], DecData[1024 + EVP_MAX_BLOCK_LENGTH];
	//每1024字节一个循环 解密文件 直到最后一个使用finally
	int Writesize = 0;
	while (TRUE) {
		//全部读取完毕
		memset(filedata, 0, 1024);
		memset(DecData, 0, 1024 + EVP_MAX_BLOCK_LENGTH);
		int readsize = fileR.Read(filedata, 1024);
		//全部读取完毕 跳出处理最后阶段
		if (readsize <= 0) {
			break;
		}
		//数据解密
		if (EVP_DecryptUpdate(&ctx, (unsigned char*)DecData, &Writesize, (unsigned char*)filedata, readsize) != 1) {
			fileR.Close();
			fileW.Close();
			EVP_CIPHER_CTX_cleanup(&ctx);
			CFile::Remove(outfile);
			return Enc_Dec_Fail;
		}
		fileW.Write(DecData, Writesize);
	}
	if (EVP_DecryptFinal_ex(&ctx, (unsigned char*)DecData, &Writesize) != 1) {
		fileR.Close();
		fileW.Close();
		CFile::Remove(outfile);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return Enc_Dec_Fail;
	}
	fileW.Write(DecData, Writesize);
	fileR.Close();
	fileW.Close();
	EVP_CIPHER_CTX_cleanup(&ctx);

	return Success;
}