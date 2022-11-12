#include "pch.h"
#include "MinMax.h"

MinMax::MinMax(Player playingAs, uint8_t depth)
		: m_playingAs(playingAs), m_maxDepth(depth) {
	DLOG(INFO) << "Initializing min max with max depth of " << std::to_string(depth);
}

MinMax::MinMax()
		: m_maxDepth(5), m_playingAs(Player::P1) {
}

Col MinMax::GetBestMove() const {
	Col chosen = m_Game->GetBoard().GetValidColumns().at(0);
	LOG(INFO) << "[MinMax]: Col " << std::to_string(chosen);
	return chosen;
}