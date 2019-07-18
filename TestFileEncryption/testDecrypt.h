#pragma once

#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Decrypt_tests)

BOOST_AUTO_TEST_CASE(Decrypt_testFile)
{
	ibex::encryption::CIbexFileEncryption encrypt("12345");
	ibex::encryption::encryptBufferData_t buff;


	unsigned long ret = encrypt.decrypt("", buff);

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_FILE_EMPTY, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_FILE_EMPTY, "decrypt dest file is empty");
}

BOOST_AUTO_TEST_CASE(Decrypt_testKeyValid)
{
	ibex::encryption::CIbexFileEncryption encrypt("12345");
	ibex::encryption::encryptBufferData_t buff;

	unsigned long ret = encrypt.decrypt("C:\\TEST\\test.txt",buff);

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_KEY_INVALID, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_KEY_INVALID, "encrypt key is invalid");
}

BOOST_AUTO_TEST_CASE(Decrypt_testNormal)
{
	ibex::encryption::CIbexFileEncryption encrypt("123456789012345678901234567890aa");
	ibex::encryption::encryptBufferData_t de_buff;
	ibex::encryption::encryptBufferData_t en_buf;
	en_buf.clear();
	for (size_t i = 0; i < 51623; i++)
	{
		en_buf.push_back('a');
	}

	unsigned long ret = encrypt.encrypt(en_buf, "C:\\TEST\\test.txt");

	ret = encrypt.decrypt("C:\\TEST\\test.txt", de_buff);

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_SUCCESS, ret);
	BOOST_CHECK_EQUAL(TRUE, en_buf == de_buff);
}

BOOST_AUTO_TEST_CASE(Decrypt_testWrongKey)
{
	ibex::encryption::CIbexFileEncryption encrypt("123456789012345678901234567890aa");
	ibex::encryption::encryptBufferData_t de_buff;
	ibex::encryption::encryptBufferData_t en_buf;
	en_buf.clear();
	for (size_t i = 0; i < 51623; i++)
	{
		en_buf.push_back('a');
	}

	unsigned long ret = encrypt.encrypt(en_buf, "C:\\TEST\\test.txt");

	encrypt.setKey("aa123456789012345678901234567890");
	ret = encrypt.decrypt("C:\\TEST\\test.txt", de_buff);

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_SUCCESS, ret);
	BOOST_CHECK_EQUAL(TRUE, en_buf == de_buff);
}

BOOST_AUTO_TEST_SUITE_END()