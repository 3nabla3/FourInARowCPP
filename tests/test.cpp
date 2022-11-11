#define BOOST_TEST_MODULE isEq
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(isEq) {
	int a = 5;
	int b = 6;
	BOOST_CHECK_EQUAL(a + 1, b);
	BOOST_CHECK_NE(a, b);
}