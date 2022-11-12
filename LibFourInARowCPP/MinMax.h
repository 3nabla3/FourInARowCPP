#pragma once

#include "Game.h"

class MinMax {
public:
	explicit MinMax(Player playingAs, uint8_t depth = 5);
	MinMax();
	~MinMax() = default;
	MinMax(const MinMax& other) = default;
	MinMax(MinMax&& other) noexcept = default;
	MinMax& operator=(const MinMax& other) = default;
	MinMax& operator=(MinMax&& other) noexcept = default;

	void SetGame(const Game& game) { m_Game = &game; }

	[[nodiscard]] Player PlayingAs() const { return m_playingAs; }

	[[nodiscard]] Col GetBestMove() const;
private:
	Player m_playingAs;
	uint8_t m_maxDepth;

	// pointer to the running game
	const Game* m_Game = nullptr;
};
