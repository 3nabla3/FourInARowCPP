#define BOOST_TEST_MODULE Comparisons
#include <boost/test/included/unit_test.hpp>
#include "pch.h"
#include "Board.h"

BOOST_AUTO_TEST_CASE(testBoard) {
	Board board;
	BOOST_CHECK_EQUAL(board.GetPiece(0, 0), BoardPiece::EMPTY);
}