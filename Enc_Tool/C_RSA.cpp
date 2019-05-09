#include "pch.h"
#include "C_RSA.h"
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
C_RSA::C_RSA(){
	error = 0;
	padding = RSA_PKCS1_PADDING;
	Pub = RSA_new();
	Pri = RSA_new();
	bits = 512;
}

C_RSA::~C_RSA(){
	RSA_free(Pub);
	RSA_free(Pri);
}

int C_RSA::GenerateKey(int m_bits, unsigned long m_e) {
	//��eת��Ϊ��������
	error = 0;
	BIGNUM* bne = BN_new(); 
	int ret = BN_set_word(bne, m_e);
	RSA* r = RSA_new();
	bits = m_bits;
	ret=RSA_generate_key_ex(r, bits,bne, NULL);
	if (ret != 1)
	{
		printf("RSA_generate_key_ex err!\n");
		error = GenerateError;
	}
	else {
		//�ֱ𱣴浽����RSA��
		Pri = RSAPrivateKey_dup(r);
		Pub = RSAPublicKey_dup(r);
	}
	RSA_free(r);
	BN_free(bne);
	return error;
}

int C_RSA::WritePriKey(char* filename, CString C_Encpwd, int CertType) {
	error = 0;
	EVP_PKEY* key = EVP_PKEY_new();
	//��Կ���ܷ�ʽ
	EVP_PKEY_set1_RSA(key, Pri);
	BIO* out = BIO_new_file(filename, "w");
	//���м�����Կ������
	if (!C_Encpwd.IsEmpty()) {
		const EVP_CIPHER* enc = NULL;
		enc = EVP_des_ede3_ofb();
		//�Ƚ��������ĵ�Cstringת��Ϊchar����
		int flen = WideCharToMultiByte(CP_ACP, 0, C_Encpwd, C_Encpwd.GetLength(), NULL, 0, NULL, NULL);
		char* encpwd = new char[flen * 2 + 1];
		WideCharToMultiByte(CP_ACP, 0, C_Encpwd, C_Encpwd.GetLength(), encpwd, flen, NULL, NULL);
		encpwd[flen] = '\0';
		if (CertType == PKCS8) {
			error = PEM_write_bio_PKCS8PrivateKey(out, key, enc, NULL, 0, NULL, encpwd);
		}
		else {
			error = PEM_write_bio_RSAPrivateKey(out, Pri, enc, NULL, NULL, NULL, encpwd);
		}
		delete[]encpwd;
	}
	//�޼�������
	else {
		if (CertType == PKCS8) {
			error = PEM_write_bio_PKCS8PrivateKey(out, key, NULL, NULL, 0, NULL, NULL);
		}
		else {
			error = PEM_write_bio_RSAPrivateKey(out, Pri, NULL, NULL, NULL, NULL, NULL);
		}
		
	}
	if (error != 1) {
		error = WriteError;
	}
	else {
		error = Success;
	}
	BIO_free(out);
	EVP_PKEY_free(key);
	return error;
}

int C_RSA::WritePubKey(char* filename,int CertType) {
	error = 0;
	BIO* out = BIO_new_file(filename, "w");
	EVP_PKEY* key = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(key, Pub);
	if (CertType == PKCS8) {
		error = PEM_write_bio_PUBKEY(out, key);
	}
	else {
		error = PEM_write_bio_RSAPublicKey(out, Pub);
	}
	if (error != 1) {
		error = WriteError;
	}
	else {
		error = Success;
	}
	EVP_PKEY_free(key);
	BIO_free(out);
	return error;
}

int C_RSA::ReadPriKey(char* filename, CString C_Decpwd, int CertType) {
	error = 0;
	BIO* in = BIO_new_file(filename, "rb");
	//BIO* in = BIO_new(BIO_s_mem());
	EVP_PKEY* key = EVP_PKEY_new();
	//��������
	RSA_free(Pri);
	Pri = RSA_new();
	if (!C_Decpwd.IsEmpty()) {
		const EVP_CIPHER* enc = NULL;
		enc = EVP_des_ede3_ofb();
		//�Ƚ��������ĵ�Cstringת��Ϊchar����
		int flen = WideCharToMultiByte(CP_ACP, 0, C_Decpwd, C_Decpwd.GetLength(), NULL, 0, NULL, NULL);
		char* decpwd = new char[flen * 2 + 1];
		WideCharToMultiByte(CP_ACP, 0, C_Decpwd, C_Decpwd.GetLength(), decpwd, flen, NULL, NULL);
		decpwd[flen] = '\0';	
		if (CertType == PKCS8) {
			PEM_read_bio_PrivateKey(in, &key, NULL, decpwd);
			Pri = EVP_PKEY_get1_RSA(key);
		}
		else {
			PEM_read_bio_RSAPrivateKey(in, &Pri, NULL, decpwd);
		}
		delete[]decpwd;
	}
	//�޼�������
	else {
		if (CertType == PKCS8) {
			PEM_read_bio_PrivateKey(in, &key, NULL, NULL);
			Pri = EVP_PKEY_get1_RSA(key);
		}
		else {
			PEM_read_bio_RSAPrivateKey(in, &Pri, NULL, NULL);
		}
	}
	if (RSA_check_key(Pri) != 1) {
		error = ReadError;
	}
	else {
		error = Success;
	}
	EVP_PKEY_free(key);
	BIO_free(in);
	return error;
}

int C_RSA::ReadPubKey(char* filename, int CertType) {
	error = Success;
	BIO* in = BIO_new_file(filename, "rb");
	EVP_PKEY* key = EVP_PKEY_new();
	//��������
	RSA_free(Pub);
	Pub = RSA_new();
	if (CertType == PKCS8) {
		PEM_read_bio_PUBKEY(in, &key, NULL, NULL);
		Pub = EVP_PKEY_get1_RSA(key);
	}
	else {
		PEM_read_bio_RSAPublicKey(in, &Pub, NULL, NULL);
	}
	if (Pub == NULL) {
		error= InitError;
	}
	EVP_PKEY_free(key);
	BIO_free(in);
	return error;
}

int C_RSA::Cstring_to_PriKey(CString PriKey, CString C_Decpwd, int CertType) {
	error = 0;
	BIO* in = BIO_new(BIO_s_mem());
	//�Ƚ�Cstring PriKeyת��ΪChar 500�ֽ��㹻����
	int keylen = WideCharToMultiByte(CP_ACP, 0, PriKey, PriKey.GetLength(), NULL, 0, NULL, NULL);
	char* PrivateKey = new char[keylen * 2 + 1];
	if (PrivateKey == NULL) {
		return InitError;
	}
	WideCharToMultiByte(CP_ACP, 0, PriKey, PriKey.GetLength(), PrivateKey, keylen, NULL, NULL);
	PrivateKey[keylen] = '\0';
	if (BIO_write(in, PrivateKey, keylen) < 0) {
		error = ReadError;
		BIO_free(in);
		delete[]PrivateKey;
		return error;
	}
	delete[]PrivateKey;
	EVP_PKEY* key = EVP_PKEY_new();
	//��������
	RSA_free(Pri);
	Pri = RSA_new();
	if (!C_Decpwd.IsEmpty()) {
		const EVP_CIPHER* enc = NULL;
		enc = EVP_des_ede3_ofb();
		//�Ƚ��������ĵ�Cstringת��Ϊchar����
		int flen = WideCharToMultiByte(CP_ACP, 0, C_Decpwd, C_Decpwd.GetLength(), NULL, 0, NULL, NULL);
		char* decpwd = new char[flen * 2 + 1];
		WideCharToMultiByte(CP_ACP, 0, C_Decpwd, C_Decpwd.GetLength(), decpwd, flen, NULL, NULL);
		decpwd[flen] = '\0';
		if (CertType == PKCS8) {
			PEM_read_bio_PrivateKey(in, &key, NULL, decpwd);
			Pri = EVP_PKEY_get1_RSA(key);
		}
		else {
			PEM_read_bio_RSAPrivateKey(in, &Pri, NULL, decpwd);
		}
		delete[]decpwd;
	}
	//�޼�������
	else {
		if (CertType == PKCS8) {
			PEM_read_bio_PrivateKey(in, &key, NULL, NULL);
			Pri = EVP_PKEY_get1_RSA(key);
		}
		else {
			PEM_read_bio_RSAPrivateKey(in, &Pri, NULL, NULL);
		}
	}
	if (RSA_check_key(Pri) != 1) {
		error = ReadError;
	}
	else {
		error = Success;
	}
	EVP_PKEY_free(key);
	BIO_free(in);
	return error;
}

int C_RSA::Cstring_to_PubKey(CString PubKey, int CertType) {
	error = Success;
	BIO* in = BIO_new(BIO_s_mem());
	//�Ƚ�Cstring PriKeyת��ΪChar 500�ֽ��㹻����
	int keylen = WideCharToMultiByte(CP_ACP, 0, PubKey, PubKey.GetLength(), NULL, 0, NULL, NULL);
	char* PublicKey = new char[keylen * 2 + 1];
	if (PublicKey == NULL) {
		return InitError;
	}
	WideCharToMultiByte(CP_ACP, 0, PubKey, PubKey.GetLength(), PublicKey, keylen, NULL, NULL);
	PublicKey[keylen] = '\0';
	//Cstring������ת��Ϊchar д�뵽BIO���ٱ���ȡ
	if (BIO_write(in, PublicKey, keylen) < 0) {
		error = ReadError;
		BIO_free(in);
		delete[]PublicKey;
		return error;
	}
	delete[]PublicKey;
	EVP_PKEY* key = EVP_PKEY_new();
	//��������
	RSA_free(Pub);
	Pub = RSA_new();
	if (CertType == PKCS8) {
		PEM_read_bio_PUBKEY(in, &key, NULL, NULL);
		Pub = EVP_PKEY_get1_RSA(key);
	}
	else {
		PEM_read_bio_RSAPublicKey(in, &Pub, NULL, NULL);
	}
	if (Pub == NULL) {
		error = InitError;
	}
	EVP_PKEY_free(key);
	BIO_free(in);
	return error;
}

int C_RSA::PriKey_to_Cstring(CString &PriKey, CString C_Encpwd, int CertType) {
	error = 0;
	EVP_PKEY* key = EVP_PKEY_new();
	//��Կ���ܷ�ʽ
	EVP_PKEY_set1_RSA(key, Pri);
	BIO* out = BIO_new(BIO_s_mem());
	//���м�����Կ������
	if (!C_Encpwd.IsEmpty()) {
		const EVP_CIPHER* enc = NULL;
		enc = EVP_des_ede3_ofb();
		//�Ƚ��������ĵ�Cstringת��Ϊchar����
		int flen = WideCharToMultiByte(CP_ACP, 0, C_Encpwd, C_Encpwd.GetLength(), NULL, 0, NULL, NULL);
		char* encpwd = new char[flen * 2 + 1];
		WideCharToMultiByte(CP_ACP, 0, C_Encpwd, C_Encpwd.GetLength(), encpwd, flen, NULL, NULL);
		encpwd[flen] = '\0';
		if (CertType == PKCS8) {
			error = PEM_write_bio_PKCS8PrivateKey(out, key, enc, NULL, 0, NULL, encpwd);
		}
		else {
			error = PEM_write_bio_RSAPrivateKey(out, Pri, enc, NULL, NULL, NULL, encpwd);
		}
		delete[]encpwd;
	}
	//�޼�������
	else {
		if (CertType == PKCS8) {
			error = PEM_write_bio_PKCS8PrivateKey(out, key, NULL, NULL, 0, NULL, NULL);
		}
		else {
			error = PEM_write_bio_RSAPrivateKey(out, Pri, NULL, NULL, NULL, NULL, NULL);
		}

	}
	if (error != 1) {
		error = WriteError;
	}
	else {
		//��BIO�Ӷ�ȡ���ڴ��в�ת��ΪCString
		int len = BIO_ctrl_pending(out);
		char* data = (char*)OPENSSL_malloc(len);
		len = BIO_read(out, data, len);
		PriKey = data;
		OPENSSL_free(data);
		error = Success;
		error = Success;
	}
	BIO_free(out);
	EVP_PKEY_free(key);
	return error;
}

int C_RSA::PubKey_to_Cstring(CString &PubKey, int CertType) {
	error = 0;
	BIO* out = BIO_new(BIO_s_mem());
	EVP_PKEY* key = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(key, Pub);
	if (CertType == PKCS8) {
		error = PEM_write_bio_PUBKEY(out, key);
	}
	else {
		error = PEM_write_bio_RSAPublicKey(out, Pub);
	}
	if (error != 1) {
		error = WriteError;
	}
	else {
		//��BIO�Ӷ�ȡ���ڴ��в�ת��ΪCString
		int len = BIO_ctrl_pending(out);
		char* data = (char*)OPENSSL_malloc(len);
		len = BIO_read(out, data, len);
		PubKey = data;
		OPENSSL_free(data);
		error = Success;
	}
	EVP_PKEY_free(key);
	BIO_free(out);
	return error;
}

void C_RSA::SetPadding(int a) {   //����Ϊ1/5/3
	//a=1/5/3
	if (a == RSA_PKCS1_PADDING || a == RSA_X931_PADDING || a == RSA_NO_PADDING) {
		padding = a;
	}
	else
	{
		printf("rsa not surport !\n");
		return;
	}
	return;
}

void C_RSA::PrintPriKey() {
	RSA_print_fp(stdout, Pri, 11);
}

void C_RSA::PrintPubKey() {
	RSA_print_fp(stdout, Pub, 11);
}

int C_RSA::DealData(CString& C_from, CString& C_to, int type, int m_padding) {
	error = 0;
	//���ܺ����ĳ���
	int len = 0;
	//500�ֽ��㹻���ڱ���1024bit ������2048bit
	unsigned char to[500], from[500];
	//�Ƚ��������ĵ�Cstringת��Ϊchar����
	int flen = WideCharToMultiByte(CP_ACP, 0, C_from, C_from.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, C_from, C_from.GetLength(), (char *)from, flen, NULL, NULL);
	from[flen] = '\0';
	//���ڴ��Base64����������
	unsigned char DecBase64from[1000];
	memset(DecBase64from, 0, sizeof(unsigned char) * 1000);
	int RSAlen = 0;
	//���ܲ���Ҫ�Գ��Ƚ��м����Ҳ���Ҫ����padding�Ӽ� 
	//���ܵĻ���Ҫ�ȶ��������ݽ���Base64����
	if (type == RSA_Pri_Enc) {
		RSAlen = RSA_size(Pri);
	}
	else if (type == RSA_Pub_Enc) {
		RSAlen = RSA_size(Pub);

	}
	else if (type == RSA_Pri_Dec) {
		DecBase64((char*)from, flen, (char*)DecBase64from, RSAlen);
		goto Dec;
	}
	else if (type == RSA_Pub_Dec) {
		DecBase64((char*)from, flen, (char*)DecBase64from, RSAlen);
		goto Dec;
	}
	else {
		error = WrongType;
		return error;
	}
	SetPadding(m_padding);//�������ģʽ
	//1��2��3��4��5
	if (padding == RSA_PKCS1_PADDING) {
		RSAlen -= 11;
	}
	else if (padding == RSA_SSLV23_PADDING) {
		flen -= 11;
	}
	else if (padding == RSA_NO_PADDING) {
		RSAlen = RSAlen;
	}
	else if (padding == RSA_PKCS1_OAEP_PADDING) {
		RSAlen = RSAlen - 2 * SHA_DIGEST_LENGTH - 2;
	}
	if (type == RSA_Pri_Enc) {
		len = RSA_private_encrypt(RSAlen, from, to, Pri, padding);
		//printf("RSA_Pri_Enc\n");
	}
	else if (type == RSA_Pub_Enc) {
		len = RSA_public_encrypt(RSAlen, from, to, Pub, padding);
		//printf("RSA_Pub_Enc\n");
	}
	goto DEAL_;
Dec:
	if (type == RSA_Pri_Dec) {
		len = RSA_private_decrypt(RSAlen, DecBase64from, to, Pri, padding);
		//printf("RSA_Pri_Dec\n");
	}
	else if (type == RSA_Pub_Dec) {
		len = RSA_public_decrypt(RSAlen, DecBase64from, to, Pub, padding);
		//printf("RSA_Pub_Dec\n");
	}
	else {
		error = WrongType;
	}
DEAL_:
	if (len <= 0) {
		//printf("���ܳ���\n");
		error = Enc_Dec_Fail;
	}
	//����Ǽ������� ����Ҫ�����Ľ���Base64���룬�����޷���ʾ
	else if (type == RSA_Pri_Enc || type == RSA_Pub_Enc) {
		unsigned char* Base64to = new unsigned char[len * 2];
		memset(Base64to, 0, sizeof(unsigned char) * len * 2);
		if (!Base64to) {
			printf("�ڴ�����ʧ��\n");
			return InitError;
		}
		EncBase64((char *)to, len, (char *)Base64to, len);
		C_to = Base64to;
		error = len;
		delete[]Base64to;
	}
	//���ܵĻ�ֱ�ӷ�������
	else {
		C_to = to;
		error = len;
	}
	return error;
}
int C_RSA::Pri_Sign(char* data, int datalen, char* sign, unsigned int& signlen,int mdtype) {
	error = Success;
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX_init(&md_ctx);
	const EVP_MD *type;
	EVP_PKEY* pkey;
	pkey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pkey, Pri);
	if (mdtype == Type_SHA1) {
		type = EVP_sha1();
	}
	else if (mdtype == Type_SHA256) {
		type = EVP_sha256();
	}
	else {
		type = EVP_md5();
	}
	if (EVP_SignInit_ex(&md_ctx, type, NULL) != 1) {
		error = InitError;
		goto sign_end;
	}
	//��������ժҪ
	if (EVP_SignUpdate(&md_ctx, (unsigned char*)data, datalen) != 1) {
		error = EVPHashError;
		goto sign_end;
	}
	//ǩ�� ��ժҪ���м���
	if (EVP_SignFinal(&md_ctx, (unsigned char*)sign, (unsigned int*)signlen, pkey) != 1) {
		error = SignFail;
		goto sign_end;
	}
sign_end:
	EVP_MD_CTX_cleanup(&md_ctx);
	return error;
}

int C_RSA::Pub_Verfiy(char* data, int datalen, char* sign, unsigned int signlen, int mdtype) {
	error = Success;
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX_init(&md_ctx);
	const EVP_MD* type;
	EVP_PKEY* pkey;
	pkey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pkey, Pub);
	if (mdtype == Type_SHA1) {
		type = EVP_sha1();
	}
	else if (mdtype == Type_SHA256) {
		type = EVP_sha256();
	}
	else {
		type = EVP_md5();
	}
	if (EVP_VerifyInit_ex(&md_ctx, type, NULL) != 1) {
		error = InitError;
		goto verify_end;
	}
	//����ժҪֵ
	if (EVP_VerifyUpdate(&md_ctx, (unsigned char*)data, datalen) != 1) {
		error = EVPHashError;
		goto verify_end;
	}
	//��֤ǩ��
	if (EVP_VerifyFinal(&md_ctx, (unsigned char*)sign, (unsigned int)signlen, pkey) != 1) {
		error = VerifyFail;
		goto verify_end;
	}
verify_end:
	EVP_MD_CTX_cleanup(&md_ctx);
	return error;
}

int C_RSA::DealData(char*from,int flen, char*to,int &tlen, int type, int m_padding) {
	error = 0;
	//���ܺ����ĳ���
	int len = 0;
	//����Ҫ����Base64 ֱ�ӶԶ����Ƽӽ���
	int RSAlen = 0;
	//���ܲ���Ҫ�Գ��Ƚ��м����Ҳ���Ҫ����padding�Ӽ� 
	//���ܵĻ���Ҫ�ȶ��������ݽ���Base64����
	if (type == RSA_Pri_Enc) {
		RSAlen = RSA_size(Pri);
	}
	else if (type == RSA_Pub_Enc) {
		RSAlen = RSA_size(Pub);

	}
	else if (type == RSA_Pri_Dec) {
		RSAlen = flen;
		goto Dec;
	}
	else if (type == RSA_Pub_Dec) {
		RSAlen = flen;
		goto Dec;
	}
	else {
		error = WrongType;
		return error;
	}
	SetPadding(m_padding);//�������ģʽ
	//1��2��3��4��5
	if (padding == RSA_PKCS1_PADDING) {
		RSAlen -= 11;
	}
	else if (padding == RSA_SSLV23_PADDING) {
		flen -= 11;
	}
	else if (padding == RSA_NO_PADDING) {
		RSAlen = RSAlen;
	}
	else if (padding == RSA_PKCS1_OAEP_PADDING) {
		RSAlen = RSAlen - 2 * SHA_DIGEST_LENGTH - 2;
	}
	if (type == RSA_Pri_Enc) {
		len = RSA_private_encrypt(RSAlen, (unsigned char*)from, (unsigned char*)to, Pri, padding);
		//printf("RSA_Pri_Enc\n");
	}
	else if (type == RSA_Pub_Enc) {
		len = RSA_public_encrypt(RSAlen, (unsigned char*)from, (unsigned char*)to, Pub, padding);
		//printf("RSA_Pub_Enc\n");
	}
	goto DEAL_;
Dec:
	if (type == RSA_Pri_Dec) {
		len = RSA_private_decrypt(RSAlen, (unsigned char*)from, (unsigned char*)to, Pri, padding);
		//printf("RSA_Pri_Dec\n");
	}
	else if (type == RSA_Pub_Dec) {
		len = RSA_public_decrypt(RSAlen, (unsigned char*)from, (unsigned char*)to, Pub, padding);
		//printf("RSA_Pub_Dec\n");
	}
	else {
		error = WrongType;
	}
DEAL_:
	if (len <= 0) {
		//printf("���ܳ���\n");
		error = Enc_Dec_Fail;
	}
	else {
		//�������ĳ���
		tlen = len;
		error = Success;
	}
	return error;
}