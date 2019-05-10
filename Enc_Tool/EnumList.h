#pragma once

enum Error {
	GenerateError = -20,
	WriteError, ReadError, InitError,
	Enc_Dec_Fail,
	EVPHashError,
	VerifyFail,SignFail,
	WrongType, DataBigError,
	KeyError,
	WrongBlockSize,
	Success = 0,
};

enum DES_AES_Type {
	ECB, CBC, OFB, CFB,
};

enum Enc_Dec {
	Dec=0, Enc = 1,
};
enum CertType {
	PKCS8,PKCS1
};

enum MDtype {
	Type_MD5, Type_SHA1, Type_SHA256
};
enum RSA_Pri_Pub_Dec_Enc {
	RSA_Pri_Enc, RSA_Pri_Dec, RSA_Pub_Enc, RSA_Pub_Dec,
};
//BASE64���� Base64����󳤶�Ϊԭ���� len/3 * 4 + 0/1/2
void EncBase64(char* charBuf, int charBufLen, char* base64Char, int& base64CharLen);      
//BASE64����
void DecBase64(char* base64Char, int base64CharLen, char* outStr, int& outStrLen);          

int SaveData(CString filepath, char* Data, ULONGLONG Datalen);

CString FileDlg_Save(char* Data, ULONGLONG Datalen);
CString FileDlg_GetSavePath();
//�ַ������з�ת��  0:windows ת unix 1:unixתwindows
CString TransData(CString A, int type);

int WriteCstringFileWC_MB(char* filename, CString data);