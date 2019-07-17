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

BOOST_AUTO_TEST_CASE(Decrypt_testcase2)
{
	ibex::encryption::CIbexFileEncryption encrypt("12345");
	ibex::encryption::encryptBufferData_t buff;

	unsigned long ret = encrypt.decrypt("C:\\TEST\\test.txt",buff);

	BOOST_CHECK_EQUAL(IBEX_ENCRYPTION_KEY_INVALID, ret);
	BOOST_CHECK_MESSAGE(ret == IBEX_ENCRYPTION_KEY_INVALID, "encrypt key is invalid");
}

BOOST_AUTO_TEST_SUITE_END()