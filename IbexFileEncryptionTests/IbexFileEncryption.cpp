#include "pch.h"
#include "IbexFileEncryption.h"
#include "define.h"
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <fstream>
#include <sstream>

namespace ibex {

	namespace encryption {

		CIbexFileEncryption::CIbexFileEncryption(const tstring &key)
		{
			m_sKey = key;
		}

		CIbexFileEncryption::~CIbexFileEncryption()
		{

		}

		unsigned long CIbexFileEncryption::encrypt(const encryptBufferData_t &_buffer, const tstring &_destFilePath)
		{
			// need check _buffer whether is empty
			if (_buffer.empty())
			{
				std::cout << "buffer invalid" << std::endl;
				return IBEX_ENCRYPTION_BUFFER_EMPTY;
			}
			if (_destFilePath.empty())
			{
				std::cout << "dest file path invalid" << std::endl;
				return IBEX_ENCRYPTION_FILE_EMPTY;
			}
			if (m_sKey.empty())
			{
				std::cout << "key value is null" << std::endl;
				return IBEX_ENCRYPTION_KEY_EMPTY;
			}

			try
			{
				// allocate a memory prepare for encrypt need smart class pointer
				int origin_len = _buffer.size();
				encryptBufferData_t encrypt_buffer(2 * origin_len);
				EVP_CIPHER_CTX ctx;
				encryptBufferData_t iv(EVP_MAX_IV_LENGTH);
				int ret;
				int total_len = 0;
				int update_len = 0;
				int final_len = 0;

				//encrypt process
				EVP_CIPHER_CTX_init(&ctx);

				ret = EVP_EncryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)m_sKey.c_str(), (unsigned char*)&iv[0]);
				if (ret != 1) {
					std::cout << "EVP_EncryptInit_ex failed" << std::endl;
					return IBEX_ENCRYPTION_INIT_FAILED;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);

				ret = EVP_EncryptUpdate(&ctx, (unsigned char*)&encrypt_buffer[0], &update_len, (unsigned char*)&_buffer[0], origin_len);
				if (ret != 1) {
					std::cout << "EVP_EncryptUpdate failed" << std::endl;
					return IBEX_ENCRYPTION_UPDATE_FAILED;
				}
				if (update_len >= 2*origin_len)
				{
					int n = update_len / (2 * origin_len);
					encrypt_buffer.resize((n+1) * 2 * origin_len);
				}

				ret = EVP_EncryptFinal_ex(&ctx, (unsigned char*)&encrypt_buffer[update_len], &final_len);
				if (ret != 1) {
					std::cout << "EVP_EncryptFinal_ex failed" << std::endl;
					return IBEX_ENCRYPTION_FINAL_FAILED;
				}

				total_len = update_len + final_len;

				// write file
				std::ofstream out;
				out.open(_destFilePath, std::ios::out | std::ios::binary);
				if (!out.is_open())
				{
					std::cout << "open dest file failed!" << std::endl;
					return IBEX_ENCRYPTION_FILE_OPEN_FAILED;
				}
				
				for (int i = 0; i < total_len; i++)
				{
					out << encrypt_buffer[i];
				}
				out.close();
			}
			catch (const std::exception& e)
			{
				std::cerr << "encrypt exception caused by: " << e.what() << std::endl;
				return IBEX_ENCRYPTION_EXCEPTION;
			}	

			return IBEX_ENCRYPTION_SUCCESS;
		}

		unsigned long CIbexFileEncryption::decrypt(const tstring &_srcFilePath, encryptBufferData_t &_buffer)
		{
			// check input parameter valid
			if (_srcFilePath.empty())
			{
				return IBEX_ENCRYPTION_FILE_EMPTY;
			}
			if (m_sKey.empty())
			{
				return IBEX_ENCRYPTION_KEY_EMPTY;
			}
			try
			{
				std::ifstream in;
				in.open(_srcFilePath, std::ios::in | std::ios::binary);
				if (!in.is_open())
				{
					std::cout << "open src file failed!" << std::endl;
					return IBEX_ENCRYPTION_FILE_OPEN_FAILED;
				}
				std::stringstream buffer;
				buffer << in.rdbuf();
				in.close();

				int encrypt_len = buffer.str().size();
				encryptBufferData_t decrypt_buff(encrypt_len);

				// decrypt process and copy decrypt data to buffer
				encryptBufferData_t iv(EVP_MAX_IV_LENGTH);
				EVP_CIPHER_CTX ctx;
				int ret;
				int total_len = 0;
				int update_len = 0;
				int final_len = 0;

				EVP_CIPHER_CTX_init(&ctx);

				ret = EVP_DecryptInit_ex(&ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)m_sKey.c_str(), (unsigned char*)&iv[0]);
				if (ret != 1) {
					std::cout << "EVP_DecryptInit_ex failed" << std::endl;
					return IBEX_ENCRYPTION_INIT_FAILED;
				}

				EVP_CIPHER_CTX_set_padding(&ctx, 0);
				ret = EVP_DecryptUpdate(&ctx, (unsigned char*)&decrypt_buff[0], &update_len, (unsigned char*)buffer.str().c_str(), encrypt_len);
				if (ret != 1) {
					std::cout << "EVP_DecryptUpdate failed" << std::endl;
					return IBEX_ENCRYPTION_UPDATE_FAILED;
				}
				if (update_len >= encrypt_len)
				{
					int n = update_len / encrypt_len;
					decrypt_buff.resize((n+1) * encrypt_len);
				}

				ret = EVP_DecryptFinal_ex(&ctx, (unsigned char*)&decrypt_buff[update_len], &final_len);
				if (ret != 1) {
					std::cout << "EVP_DecryptFinal_ex failed" << std::endl;
					return IBEX_ENCRYPTION_FINAL_FAILED;
				}
				total_len = update_len + final_len;

				_buffer.clear();
				for (int i = 0; i < total_len; i++)
				{
					_buffer.push_back(decrypt_buff[i]);
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "exception caused by: " << e.what() << std::endl;
				return IBEX_ENCRYPTION_EXCEPTION;
			}

			return IBEX_ENCRYPTION_SUCCESS;
		}

	} //namespace encryption

} //namespace ibex
