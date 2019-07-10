#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

namespace ibex {

	namespace encryption {

		class CIbexFileEncryption
		{
			typedef std::vector<unsigned char> encryptBufferData_t;
			typedef std::basic_string<TCHAR> tstring;

		public:
			CIbexFileEncryption(const tstring &key);
			~CIbexFileEncryption();
			void encrypt(const encryptBufferData_t &_buffer, const tstring &_destFilePath);
			void decrypt(const tstring &_srcFilePath, encryptBufferData_t &_buffer);
		private:
			tstring m_sKey;
		};

	} //namespace encryption

} //namespace ibex


