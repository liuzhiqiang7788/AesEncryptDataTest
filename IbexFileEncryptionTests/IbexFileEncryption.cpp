#include "pch.h"
#include "IbexFileEncryption.h"
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>


namespace ibex {

	namespace encryption {

		CIbexFileEncryption::CIbexFileEncryption(const string &key)
		{
			m_sKey = key;
		}

		CIbexFileEncryption::~CIbexFileEncryption()
		{

		}

		void CIbexFileEncryption::encrypt(const IFileEncryption_t &_buffer, const tstring &_destFilePath)
		{
			try
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
					return;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);

				ret = EVP_EncryptUpdate(&ctx, encrypt, &mlen, data, AES_BLOCK_SIZE * 3);
				if (ret != 1) {
					cout << "EVP_EncryptUpdate failed" << endl;
					return;
				}

				ret = EVP_EncryptFinal_ex(&ctx, encrypt + mlen, &flen);
				if (ret != 1) {
					cout << "EVP_EncryptFinal_ex failed" << endl;
					return;
				}

				tlen = mlen + flen;

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
			}
			catch (const std::exception&)
			{

			}	
		}

		void CIbexFileEncryption::decrypt(const tstring &_srcFilePath, IFileEncryption_t &_buffer)
		{
			try
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

				EVP_CIPHER_CTX_cleanup(&ctx);
				EVP_CIPHER_CTX_init(&ctx);

				ret = EVP_DecryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)userkey, iv);
				if (ret != 1) {
					cout << "EVP_DecryptInit_ex failed" << endl;
					return;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);
				ret = EVP_DecryptUpdate(&ctx, plain, &mlen, encrypt, AES_BLOCK_SIZE * 3);
				if (ret != 1) {
					cout << "EVP_DecryptUpdate failed" << endl;
					return;
				}

				ret = EVP_DecryptFinal_ex(&ctx, plain + mlen, &flen);
				if (ret != 1) {
					cout << "EVP_DecryptFinal_ex failed" << endl;
					return;
				}

				if (!memcmp(plain, data, AES_BLOCK_SIZE * 3)) {
					cout << "decprypt data equal with oringin data" << endl;
				}
				else {
					cout << "decrypt data different with oringin data" << endl;
					return;

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
			}
			catch (const std::exception&)
			{
			
			}
			
			return;
		}

	} //namespace encryption

} //namespace ibex
