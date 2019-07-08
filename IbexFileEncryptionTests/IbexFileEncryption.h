#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace ibex {

	namespace encryption {

		class CIbexFileEncryption
		{
			typedef vector<unsigned char> IFileEncryption_t;
			typedef std::basic_string<char> tstring;

		public:
			CIbexFileEncryption(const string &key);
			~CIbexFileEncryption();
			void encrypt(const IFileEncryption_t &_buffer, const tstring &_destFilePath);
			void decrypt(const tstring &_srcFilePath, IFileEncryption_t &_buffer);
		private:
			string m_sKey;
		};

	} //namespace encryption

} //namespace ibex


