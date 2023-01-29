#include "pch.h"
#include "TreeNode.h"
#include "Game.h"

TreeNode::TreeNode(Board board, bool isMaximizing, Col delta)
		: m_Board(std::move(board)), m_IsMaximizing(isMaximizing), m_delta(delta) {
}

Score TreeNode::GetScore() const {
	if (!m_Score)
		m_Score = MinMax(std::numeric_limits<Score>::min(), std::numeric_limits<Score>::max());
	return m_Score.value();
}

GameState TreeNode::GetGameState() const {
	if (!m_gameState)
		m_gameState = Game::AnalyzeGameState(m_Board);
	return m_gameState.value();
}

uint32_t TreeNode::GetNumNodes() const {
	uint32_t sum = 1;

	for (const auto& child : m_Children)
		sum += child->GetNumNodes();
	return sum;
}

std::shared_ptr<TreeNode>& TreeNode::GetChildFromDelta(uint8_t index) {
	for (auto& child : m_Children)
		if (child->GetDelta() == index)
			return child;

	LOG(FATAL) << "Could not find child from delta";
}

uint32_t TreeNode::NumNodesInFullTree(int nPerLayer, int depth) {
	/// Return the sum 1 + nPerLayer + nPerLayer^2 + ... + nPerLayer^depth
	uint32_t sum = 0;
	for (int i = 0; i <= depth; i++)
		sum += static_cast<uint32_t>(std::pow(nPerLayer, i));
	return sum;
}


void TreeNode::AddLayer() {
	/// Add a layer to all the IN_PROGRESS leave nodes
	/// to update the tree from the last play

	// reset the score of the node, so it can be calculated again
	m_Score = std::nullopt;

	// loop will not execute if the node is a leaf, no need for if statement
	for (const auto& child : m_Children)
		child->AddLayer();

	// only extend the branches that are not game over
	if (IsLeaf() && GetGameState() == GameState::IN_PROGRESS)
		GenerateTree(1);
}

void TreeNode::GenerateTree(uint8_t depth) {
	if (depth <= 0) return; // exit condition

	// for each new possible play, create a child
	for (Col col : m_Board.GetValidColumns()) {
		Board newBoard = m_Board;
		newBoard.InsertPiece(col, Game::ToPiece(GetPlaysNext()));
		m_Children.push_back(std::make_shared<TreeNode>(std::move(newBoard), !m_IsMaximizing, col));
	}
	// make each child generate its own tree, if the game has not ended yet
	for (auto& child : m_Children) {
		// if the game has not ended yet
		if (child->GetGameState() == GameState::IN_PROGRESS)
			child->GenerateTree(depth - 1);
	}
}

Score TreeNode::MinMax(Score alpha, Score beta) const {
	/// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
	if (IsLeaf())
		return CalculateStaticScore();

	// the minimizing and maximizing branches have been merged for clarity
	Score bestVal = WorstScoreOf(std::numeric_limits<Score>::min(), std::numeric_limits<Score>::max());
	for (const auto& child: m_Children) {
		Score value = child->MinMax(alpha, beta);
		bestVal = BestScoreOf(bestVal, value);
		if (IsMaximizing())
			alpha = BestScoreOf(alpha, bestVal);
		else
			beta = BestScoreOf(beta, bestVal);
		if (beta <= alpha)
			break;
	}
	return bestVal;
}

Score TreeNode::CalculateStaticScore() const {
	if (m_gameState == GameState::P1_WON) return std::numeric_limits<Score>::max();
	if (m_gameState == GameState::P2_WON) return std::numeric_limits<Score>::min();
	if (m_gameState == GameState::TIE) return 0;
	return static_cast<Score>(CalculateStaticPlayerScore(Player::P1) - CalculateStaticPlayerScore(Player::P2));
}

/// Simple macro used to write less code in the function below,
///  Simply analyzes each line of the given generation function and keeps track of the longestLength;
#define CheckDirection(genFn, maxIdx) \
for (uint8_t idx = 0; idx < (maxIdx); idx++) { \
	std::vector<BoardPiece> line = genFn(idx); \
	totalLength += AnalyzeLine(line, player); \
}

Score TreeNode::CalculateStaticPlayerScore(Player player) const {
	// TODO: improve the static analysis to make sure the algorithm gets a more
	//  detailed view of the game
	Score rowLength = 0, colLength = 0, upDiagLength = 0, dnDiagLength = 0;

	for (uint8_t idx = 0; idx < Board::N_ROWS; idx++)
		rowLength = (Score)(rowLength + AnalyzeLine(m_Board.GetRow(idx), player));
	for (uint8_t idx = 0; idx < Board::N_COLS; idx++) {
		const auto& temp = m_Board.GetCol(idx);
		colLength = (Score) (colLength + AnalyzeLine(temp, player));
	}
	for (uint8_t idx = 0; idx < (uint8_t) (Board::N_COLS + Board::N_ROWS - 1); idx++)
		upDiagLength = (Score)(upDiagLength + AnalyzeLine(m_Board.GetUpDiag(idx), player));
	for (uint8_t idx = 0; idx < (uint8_t) (Board::N_COLS + Board::N_ROWS - 1); idx++)
		dnDiagLength = (Score)(dnDiagLength + AnalyzeLine(m_Board.GetDnDiag(idx), player));

	auto totalLength = (Score)(rowLength + colLength + upDiagLength + dnDiagLength);
	return (Score)totalLength;
}

bool TreeNode::FourInARowImpossible(std::vector<BoardPiece>::const_iterator begin,
									std::vector<BoardPiece>::const_iterator end,
									Player player) {
	// an alignment is impossible if the line is less than 4
	// or the sum of empty and player pieces is less than 4
	return (end - begin) < 4 ||
		   std::count(begin, end, BoardPiece::EMPTY) +
		   std::count(begin, end, Game::ToPiece(player)) < 4;
}

/// Return 4 - (the minimum number of pieces the player has to add to the line
///  in order to get a 4 in a row on that line). If there is not enough space to get a 4
///  in a row, return 0
Score TreeNode::AnalyzeLine(const std::vector<BoardPiece>& line, Player player) {
	// if making a four in a row is impossible, return immediately
	if (FourInARowImpossible(line.begin(), line.end(), player))
		return 0;

	// split the line on every opponent's piece and analyze each segment independently
	BoardPiece otherPiece = player == Player::P1 ? BoardPiece::P2 : BoardPiece::P1;
	std::vector<Score> results; // holds the results of AnalyzeLine_Impl for each segment
	auto segmentStart = line.begin();
	while (*segmentStart == otherPiece)
		segmentStart++;
	for (auto it = segmentStart; it != std::end(line); it++) {
		if (*it != otherPiece) continue;
		auto segmentEnd = it; // the end is exclusive
		results.push_back(AnalyzeLine_Impl(segmentStart, segmentEnd, player));
		segmentStart = segmentEnd;  // set the start of the next segment
	}
	// calculate the last segment once we've reached the end of the line
	results.push_back(AnalyzeLine_Impl(segmentStart, line.end(), player));

	return *std::max_element(results.begin(), results.end());
}

Score TreeNode::AnalyzeLine_Impl(std::vector<BoardPiece>::const_iterator begin,
								 std::vector<BoardPiece>::const_iterator end, Player player) {

	if (FourInARowImpossible(begin, end, player))
		return 0;

	uint8_t pre = 0, chain = 0, post = 0;
	auto size = end - begin;
	uint8_t inner[size / 2]; // the number of inner spaces should not be greater than that
	std::fill(inner, inner + sizeof inner, 0); // 0 out the array
	uint8_t innerIdx = 0; // used to append to inner

	for (auto it = begin; it != end; it++) {
		// deal with the empty spots first
		if (*it == BoardPiece::EMPTY) {
			if (chain == 0) pre++;
			else post++;
			continue;
		}

		chain++;
		// if we are hitting another chain
		if (post != 0) {
			inner[innerIdx++] = post;
			post = 0;
		}
	}

	// if the line is split in two
	if (innerIdx) {
		int sumInner = 0;
		for (unsigned char i : inner) { sumInner += i; }

		// if we don't need to extent to the sides
		if (chain + sumInner >= 4)
			return (Score) (4 - sumInner);
		// if we do
		if (pre + chain + sumInner + post >= 4)
			return (Score) chain;
	}
	// if the chain is in one piece, and we can extend it to 4
	else if (pre + chain + post >= 4)
		return (Score) chain;
	// if we cannot find a chain that can be extended to 4
	return 0;
}

