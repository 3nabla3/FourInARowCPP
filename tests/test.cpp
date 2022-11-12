#define BOOST_TEST_MODULE isEq
#include <boost/test/included/unit_test.hpp>
#include "LibFourInARowCPP/Board.h"

BOOST_AUTO_TEST_CASE(testBoard) {
	Board board;
	auto piece = board.GetPiece(0, 0);
	BOOST_CHECK_EQUAL(piece, BoardPiece::EMPTY);
}