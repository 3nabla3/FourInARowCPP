#define BOOST_TEST_MODULE test_StaticAnalysis
#include <boost/test/included/unit_test.hpp>
#include "LibFourInARowCPP/GameEnums.h"
#include "LibFourInARowCPP/Board.h"
#include "LibFourInARowCPP/TreeNode.h"

BOOST_AUTO_TEST_CASE(test_Board) {
	Board board;
	auto piece = board.GetPiece(0, 0);
	BOOST_CHECK_EQUAL(piece, BoardPiece::EMPTY);
}

BOOST_AUTO_TEST_CASE(test_AnalyzeLine1) {
	using BoardPiece::EMPTY;
	using BoardPiece::P1;
	using BoardPiece::P2;
	// convert each score to an int to make it print as an integer not a char
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY},	Player::P1), 0);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	EMPTY,	EMPTY,	P1,		EMPTY,	EMPTY},	Player::P1), 1);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	EMPTY,	P1,		P1,		EMPTY,	EMPTY},	Player::P1), 2);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	P1,		P1,		P1,		EMPTY,	EMPTY},	Player::P1), 3);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	P1,		P1,		P2,		P1,		P1},	Player::P1), 0);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	P1,		EMPTY,	P1,		P1,		EMPTY},	Player::P1), 3);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	P1,		EMPTY,	P1,		P1,		P2},	Player::P1), 3);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({P1,		EMPTY,	P1,		EMPTY,	P1,		P2},	Player::P1), 2);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({P1,		P1,		EMPTY,	P1,		P1,		EMPTY},	Player::P1), 3);
}

BOOST_AUTO_TEST_CASE(test_AnalyzeLine2) {
	using BoardPiece::EMPTY;
	using BoardPiece::P1;
	using BoardPiece::P2;
	Score score = TreeNode::AnalyzeLine({EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	P2},	Player::P1);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({P2,		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY},	Player::P1), 0);
	BOOST_CHECK_EQUAL((int)TreeNode::AnalyzeLine({EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	P2},	Player::P1), 0);
}