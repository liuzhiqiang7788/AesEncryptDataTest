#pragma once
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(FileIO_tests)

BOOST_AUTO_TEST_CASE(FileIO_testcase1)
{
	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(FileIO_testcase2)
{
	BOOST_TEST(false);
}

BOOST_AUTO_TEST_SUITE_END()