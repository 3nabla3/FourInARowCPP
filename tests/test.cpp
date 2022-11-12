#define BOOST_TEST_MODULE myModule
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(isEq) {
	int a = 5;
	int b = 6;
	BOOST_CHECK_EQUAL(a + 1, b);
	BOOST_CHECK_NE(a, b);
}

BOOST_AUTO_TEST_CASE(isNotEq) {
	int a = 10;
	int b = 10;
	BOOST_CHECK_EQUAL(a, b);
	BOOST_CHECK_EQUAL(a, 10);
	BOOST_CHECK_EQUAL(9, b - 1);
}