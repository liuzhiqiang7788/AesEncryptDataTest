#include "pch.h"
#include "IbexFileEncryption.h"
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <fstream>

namespace ibex {

	namespace encryption {

		CIbexFileEncryption::CIbexFileEncryption(const tstring &key)
		{
			m_sKey = key;
		}

		CIbexFileEncryption::~CIbexFileEncryption()
		{

		}

		void CIbexFileEncryption::encrypt(const IEncryptionData_t &_buffer, const tstring &_destFilePath)
		{
			// need check _buffer whether is empty
			if (_buffer.empty() || _destFilePath.empty())
			{
				cout << "input parameter invalid" << endl;
				return;
			}
			if (m_sKey.empty())
			{
				cout << "key value is null" << endl;
				return;
			}

			try
			{
				// allocate a memory prepare for encrypt need smart class pointer
				int origin_len = _buffer.size();
				IEncryptionData_t origin_buffer = _buffer;
				std::shared_ptr<unsigned char[]> encrypt_buffer(new unsigned char[2 * origin_len]);
				EVP_CIPHER_CTX ctx;
				IEncryptionData_t iv(EVP_MAX_IV_LENGTH);
				int ret;
				int total_len = 0;
				int update_len = 0;
				int final_len = 0;

				//encrypt process
				EVP_CIPHER_CTX_init(&ctx);

				ret = EVP_EncryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)m_sKey.c_str(), (unsigned char*)&iv[0]);
				if (ret != 1) {
					cout << "EVP_EncryptInit_ex failed" << endl;
					return;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);

				ret = EVP_EncryptUpdate(&ctx, (unsigned char*)encrypt_buffer[0], &update_len, (unsigned char*)origin_buffer[0], origin_len);
				if (ret != 1) {
					cout << "EVP_EncryptUpdate failed" << endl;
					return;
				}

				ret = EVP_EncryptFinal_ex(&ctx, (unsigned char*)&encrypt_buffer[update_len], &final_len);
				if (ret != 1) {
					cout << "EVP_EncryptFinal_ex failed" << endl;
					return;
				}

				total_len = update_len + final_len;

				// write file
				ofstream out(_destFilePath, fstream::out);
				out.open(_destFilePath, ios::in | ios::out | ios::binary);
				if (out.is_open())
				{
					out << (unsigned char*)&encrypt_buffer[0];
					out.close();
				}
			}
			catch (const std::exception& e)
			{
				cout << "encrypt exception caused by: " << e.what() << endl;
				return;
			}	
		}

		void CIbexFileEncryption::decrypt(const tstring &_srcFilePath, IEncryptionData_t &_buffer)
		{
			// check input parameter valid
			if (_srcFilePath.empty())
			{
				return;
			}
			if (m_sKey.empty())
			{
				return;
			}
			try
			{
				// allocate memory
				IEncryptionData_t encrypt_buff;
				// read file and copy to memory
				//encrypt_buff.push_back();

				int encrypt_len = encrypt_buff.size();
				IEncryptionData_t decrypt_buff(encrypt_len);

				// decrypt process and copy decrypt data to buffer
				IEncryptionData_t iv(EVP_MAX_IV_LENGTH);
				EVP_CIPHER_CTX ctx;
				int ret;
				int total_len = 0;
				int update_len = 0;
				int final_len = 0;

				EVP_CIPHER_CTX_init(&ctx);

				ret = EVP_DecryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)m_sKey.c_str(), (unsigned char*)&iv[0]);
				if (ret != 1) {
					cout << "EVP_DecryptInit_ex failed" << endl;
					return;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);
				ret = EVP_DecryptUpdate(&ctx, (unsigned char*)&decrypt_buff[0], &update_len, (unsigned char*)&encrypt_buff[0], encrypt_len);
				if (ret != 1) {
					cout << "EVP_DecryptUpdate failed" << endl;
					return;
				}

				ret = EVP_DecryptFinal_ex(&ctx, (unsigned char*)&decrypt_buff[update_len], &final_len);
				if (ret != 1) {
					cout << "EVP_DecryptFinal_ex failed" << endl;
					return;
				}

				_buffer = decrypt_buff;

				//free memory and close fd , EVP handl close

			}
			catch (const std::exception& e)
			{
				cout << "exception caused by: " << e.what() << endl;
				return;
			}
		}

	} //namespace encryption

} //namespace ibex
