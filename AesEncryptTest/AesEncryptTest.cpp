// AesEncryptTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <openssl/aes.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <openssl/evp.h>
#include <string.h>

using namespace std;

unsigned long AES_Encpty_Decrypt_Data();
unsigned long EVP_Encpty_Decrypt_Data();

int main()
{
	int ret = 0;
	ret = EVP_Encpty_Decrypt_Data();
	if (0 == ret)
	{
		cout << "use EVP process success!" << endl;
	}

	ret = AES_Encpty_Decrypt_Data();
	if (0 == ret)
	{
		cout << "use AES process success!" << endl;
	}

	system("pause");

	exit(0);
}

unsigned long EVP_Encpty_Decrypt_Data()
{
	char userkey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char *data = (unsigned char*)malloc(AES_BLOCK_SIZE * 3);
	unsigned char *encrypt = (unsigned char*)malloc(AES_BLOCK_SIZE * 6);
	unsigned char *plain = (unsigned char*)malloc(AES_BLOCK_SIZE * 6);
	EVP_CIPHER_CTX ctx;
	int ret;
	int tlen = 0;
	int mlen = 0;
	int flen = 0;

	memset((void*)userkey, 'k', EVP_MAX_KEY_LENGTH);
	memset((void*)iv, 'i', EVP_MAX_IV_LENGTH);
	memset((void*)data, 'p', AES_BLOCK_SIZE * 3);
	memset((void*)encrypt, 0, AES_BLOCK_SIZE * 6);
	memset((void*)plain, 0, AES_BLOCK_SIZE * 6);

	/*init ctx*/
	EVP_CIPHER_CTX_init(&ctx);

	ret = EVP_EncryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)userkey, iv);
	if (ret != 1) {
		cout << "EVP_EncryptInit_ex failed" << endl;
		return -1;
	}

	EVP_CIPHER_CTX_set_padding(&ctx, 0);

	ret = EVP_EncryptUpdate(&ctx, encrypt, &mlen, data, AES_BLOCK_SIZE * 3);
	if (ret != 1) {
		cout << "EVP_EncryptUpdate failed" << endl;
		return -1;
	}

	ret = EVP_EncryptFinal_ex(&ctx, encrypt + mlen, &flen);
	if (ret != 1) {
		cout << "EVP_EncryptFinal_ex failed" << endl;
		return -1;
	}

	tlen = mlen + flen;

	tlen = 0;
	mlen = 0;
	flen = 0;

	EVP_CIPHER_CTX_cleanup(&ctx);
	EVP_CIPHER_CTX_init(&ctx);

	ret = EVP_DecryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)userkey, iv);
	if (ret != 1) {
		cout << "EVP_DecryptInit_ex failed" << endl;
		return -1;
	}

	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	ret = EVP_DecryptUpdate(&ctx, plain, &mlen, encrypt, AES_BLOCK_SIZE * 3);
	if (ret != 1) {
		cout << "EVP_DecryptUpdate failed" << endl;
		return -1;
	}

	ret = EVP_DecryptFinal_ex(&ctx, plain + mlen, &flen);
	if (ret != 1) {
		cout << "EVP_DecryptFinal_ex failed" << endl;
		return -1;
	}

	if (!memcmp(plain, data, AES_BLOCK_SIZE * 3)) {
		cout << "decprypt data equal with oringin data" << endl;
	}
	else {
		cout << "decrypt data different with oringin data" << endl;
		return -1;

	}

	if (NULL != data)
	{
		free(data);
		data = NULL;
	}

	if (NULL != encrypt)
	{
		free(encrypt);
		encrypt = NULL;
	}

	if (NULL != plain)
	{
		free(plain);
		plain = NULL;
	}

	return 0;
}

unsigned long AES_Encpty_Decrypt_Data()
{
	string strSource = "1234567890abcde";

	string strEncryptData;
	string strDecryptData;

	string strKey = "0123456789abcdef0123456789abcdef";

	vector<unsigned char> ivec(AES_BLOCK_SIZE);

	AES_KEY AesKey;

	int DataLen = strSource.size();
	int SetDataLen = 0;

	// set the encryption length
	if ((DataLen%AES_BLOCK_SIZE) == 0)
	{
		SetDataLen = DataLen;
	}
	else
	{
		SetDataLen = ((DataLen / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
	}
	cout << "SetDataLen:" << SetDataLen << endl;

	//set encrypt key
	memset(&AesKey, 0x00, sizeof(AES_KEY));
	if (AES_set_encrypt_key((const unsigned char*)strKey.c_str(), 256, &AesKey) < 0)
	{
		cerr << "Unable to set encryption key in AES" << endl;
		return -1;
	}

	for (size_t i = 0; i < AES_BLOCK_SIZE; i++)
	{
		ivec[i] = 0;
	}

	//encrypt
	AES_cbc_encrypt((unsigned char *)strSource.c_str(), (unsigned char*)strEncryptData.c_str(),
		SetDataLen, &AesKey, (unsigned char*)&ivec[0], AES_ENCRYPT);

	//set decrypt key
	memset(&AesKey, 0x00, sizeof(AES_KEY));
	int ret = 0;
	if (ret = AES_set_decrypt_key((const unsigned char*)strKey.c_str(), 256, &AesKey) < 0)
	{
		cerr << "Unable to set decryption key in AES" << endl;
		return -1;
	}


	for (size_t i = 0; i < AES_BLOCK_SIZE; i++)
	{
		ivec[i] = 0;
	}

	//decryption
	AES_cbc_encrypt((unsigned char *)strEncryptData.c_str(), (unsigned char *)strDecryptData.c_str(),
		SetDataLen, &AesKey, (unsigned char*)&ivec[0], AES_DECRYPT);

	cout << "Decrypt Data: " << strDecryptData.c_str() << endl;
	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
