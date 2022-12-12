#include "pch.h"
#include "MinMax.h"
#include "Game.h"

MinMax::MinMax(Player playingAs, uint8_t depth, Board* gameBoard)
		: m_playingAs(playingAs), m_maxDepth(depth), m_Board(gameBoard) {
	DLOG(INFO) << "Initializing MinMax with max depth of " << std::to_string(m_maxDepth) << "...";
}

void MinMax::GenerateTree() {
	if (!m_Board)
		LOG(FATAL) << "The board has not been set!";

	DLOG(INFO) << "Generating tree of depth " << std::to_string(m_maxDepth) << "..." << std::endl;
	bool maximizing = Game::GetPlaysNext(*m_Board) == Player::P1;
	m_head = std::make_shared<TreeNode>(*m_Board, maximizing);
	m_head->GenerateTree(m_maxDepth);
	DLOG(INFO) << "Finished generating tree";
}

void MinMax::AddLayer() {
	DLOG(INFO) << "Updating next layer of tree..." << std::endl;
	m_head->UpdateTree();
	DLOG(INFO) << "Finished updating tree";
}

/// Shift the new head of the tree to be one of its children
void MinMax::ShiftTree(Col col) {
	std::shared_ptr<TreeNode>& newHead = m_head->GetChildFromDelta(col);
	m_head.swap(newHead);
}

Col MinMax::GetBestMove() const {
	const bool maximizing = m_playingAs == Player::P1;

	// Initialize the best score to its worst case: + or - infinity depending on the maximizing value
	Score bestScore = maximizing ? std::numeric_limits<Score>::min() : std::numeric_limits<Score>::max();
	const TreeNode* bestChildren[m_head->GetChildren().size()];
	int idx = 0;

	if (maximizing)
		LOG(INFO) << "Calculated scores: higher is better for MinMax";
	else
		LOG(INFO) << "Calculated scores: lower is better for MinMax";

	for (const auto& child: m_head->GetChildren()) {
		Score childScore = child->GetScore(m_maxDepth);
		LOG(INFO) << "Column " << std::to_string(child->GetDelta()) << " -- score: " << std::to_string(childScore);
		// if the score is equal to the best, append to the list of best children
		if (bestScore == childScore)
			bestChildren[idx++] = child.get();

		// if the score is better than the best score, reset the list and append the child
		else if (maximizing && bestScore < childScore ||
		!maximizing && bestScore > childScore) {
			idx = 0;
			bestScore = childScore;
			bestChildren[idx++] = child.get();
		}
	}

	if (!idx) {
		// should never happen, only for debugging purposes
		LOG(FATAL) << "Could not find a best move!";
		exit(1);
	}

	const int chosenIdx = ChoseRandomChild(bestChildren, idx);

	Col bestMove = bestChildren[chosenIdx]->GetDelta();
	LOG(INFO) << "Chosen move: " << std::to_string(bestMove);

	return bestMove;
}

/// Return a random child from the list of children
int MinMax::ChoseRandomChild(const TreeNode** children, int size) {
	std::stringstream ss;
	for (int i = 0; i < size; i++) {
		ss << std::to_string(children[i]->GetDelta());
		if (i != size - 1)
			ss << ", ";
	}
	LOG(INFO) << "Choosing from [ " << ss.str() << " ]";

	// crate a PRNG
	static thread_local std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, size - 1);
	// get a random index from the list of children
	int chosenIdx = dist(gen);
	// return the chosen child
	return chosenIdx;
}