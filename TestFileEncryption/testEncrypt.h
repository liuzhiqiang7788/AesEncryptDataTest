#pragma once
#include <boost/test/included/unit_test.hpp>
#include "../IbexFileEncryptionTests/define.h"
#include "../IbexFileEncryptionTests/IbexFileEncryption.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(Encrypt_tests)

BOOST_AUTO_TEST_CASE(Encrypt_testFile)
{
	ibex::encryption::CIbexFileEncryption encrypt("12345");
	ibex::encryption::encryptBufferData_t buff;
	for (size_t i = 0; i < 10; i++)
	{
		buff.push_back('a');
	}
	
	unsigned long ret = encrypt.encrypt(buff,"");

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_FILE_EMPTY, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_FILE_EMPTY, "encrypt dest file is empty");
}

BOOST_AUTO_TEST_CASE(Encrypt_testKey)
{
	ibex::encryption::CIbexFileEncryption encrypt("");
	ibex::encryption::encryptBufferData_t buff;
	for (size_t i = 0; i < 10; i++)
	{
		buff.push_back('a');
	}

	unsigned long ret = encrypt.encrypt(buff, "C:\\TEST\\test.txt");

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_KEY_INVALID, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_KEY_INVALID, "encrypt key is invalid");
}

BOOST_AUTO_TEST_CASE(Encrypt_testBuffer)
{
	ibex::encryption::CIbexFileEncryption encrypt("12345");
	ibex::encryption::encryptBufferData_t buff;

	unsigned long ret = encrypt.encrypt(buff, "C:\\TEST\\test.txt");

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_BUFFER_EMPTY, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_BUFFER_EMPTY, "encrypt buffer is empty");
}

BOOST_AUTO_TEST_CASE(Encrypt_testNormal)
{
	ibex::encryption::tstring keys = "123456789012345678901234567890aa";
	ibex::encryption::CIbexFileEncryption *encrypt = new ibex::encryption::CIbexFileEncryption(keys);
	ibex::encryption::encryptBufferData_t buff;
	for (size_t i = 0; i < 20; i++)
	{
		buff.push_back('a');
	}

	unsigned long ret = encrypt->encrypt(buff, "C:\\TEST\\test.txt");

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_SUCCESS, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_SUCCESS, "encrypt buffer success");

	delete encrypt;
}

BOOST_AUTO_TEST_SUITE_END()
