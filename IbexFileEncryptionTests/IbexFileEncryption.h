#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace ibex {

	namespace encryption {

		class CIbexFileEncryption
		{
			typedef vector<unsigned char> IEncryptionData_t;
			typedef std::basic_string<char> tstring;

		public:
			CIbexFileEncryption(const tstring &key);
			~CIbexFileEncryption();
			void encrypt(const IEncryptionData_t &_buffer, const tstring &_destFilePath);
			void decrypt(const tstring &_srcFilePath, IEncryptionData_t &_buffer);
		private:
			tstring m_sKey;
		};

	} //namespace encryption

} //namespace ibex


