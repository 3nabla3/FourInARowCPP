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

	LOG(INFO) << "Generating tree of depth " << std::to_string(m_maxDepth) << "..." << std::endl;
	bool maximizing = Game::GetPlaysNext(*m_Board) == Player::P1;
	m_head = std::make_shared<TreeNode>(*m_Board, maximizing);
	m_head->GenerateTree(m_maxDepth);
	LOG(INFO) << "Finished generating tree";
}

void MinMax::AddLayer(int count) {
	LOG(INFO) << "Updating next layer of tree..." << std::endl;
	for (int i = 0; i < count; i++)
		m_head->AddLayer();
	LOG(INFO) << "Finished updating tree";
}

void MinMax::Reset() {
	m_head.reset();
}

/// Shift the new head of the tree to be one of its children
void MinMax::ShiftTree(Col col) {
	m_head = m_head->GetChildFromDelta(col);;
}

static std::string ScoreToString(Score score) {
	if (score == std::numeric_limits<Score>::max())
		return "Player 1 wins";
	else if (score == std::numeric_limits<Score>::min())
		return "Player 2 wins";
	else
		return std::to_string(score);
}

Col MinMax::GetBestMove() const {
	const bool maximizing = m_playingAs == Player::P1;

	// Initialize the best score to its worst case: + or - infinity depending on the maximizing value
	Score bestScore = maximizing ? std::numeric_limits<Score>::min() : std::numeric_limits<Score>::max();
	const TreeNode* bestChildren[m_head->GetChildren().size()];
	int bestChildrenIdx = 0;

	LOG(INFO) << "#######################################";
	if (maximizing)
		LOG(INFO) << "higher is better for MinMax";
	else
		LOG(INFO) << "lower is better for MinMax";
	LOG(INFO) << "General\t--> score: " << ScoreToString(m_head->GetScore());
	LOG(INFO) << "---------------------------------------";

	for (const auto& child: m_head->GetChildren()) {
		Score childScore = child->GetScore();
		LOG(INFO) << "Column " << std::to_string(child->GetDelta()) << "\t--> score: " << ScoreToString(childScore);

		// if the score is equal to the best, append to the list of best children
		if (bestScore == childScore)
			bestChildren[bestChildrenIdx++] = child.get();

		// if the score is better than the best score, reset the list and append the child
		else if (maximizing && bestScore < childScore ||
		!maximizing && bestScore > childScore) {
			bestChildrenIdx = 0;
			bestScore = childScore;
			bestChildren[bestChildrenIdx++] = child.get();
		}
	}

	if (!bestChildrenIdx) {
		// should never happen, only for debugging purposes
		LOG(FATAL) << "Could not find a best move!";
		exit(1);
	}

	const int chosenIdx = ChoseRandomChild(bestChildren, bestChildrenIdx);

	Col bestMove = bestChildren[chosenIdx]->GetDelta();
	LOG(INFO) << "Chosen move: " << std::to_string(bestMove);
	LOG(INFO) << "#######################################";

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