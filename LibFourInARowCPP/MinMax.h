#pragma once

#include "Board.h"
#include "Player.h"

class MinMax {
public:
	MinMax();
	explicit MinMax(Player playingAs, uint8_t depth = 5);

	~MinMax() = default;
	MinMax(const MinMax& other) = default;
	MinMax(MinMax&& other) noexcept = default;
	MinMax& operator=(const MinMax& other) = default;
	MinMax& operator=(MinMax&& other) noexcept = default;

	void SetBoard(Board* board) { m_Board = board; }

	[[nodiscard]] Player PlayingAs() const { return m_playingAs; }

	[[nodiscard]] Col GetBestMove() const;
private:
	Player m_playingAs;
	uint8_t m_maxDepth;

	// pointer to the running game
	const Board* m_Board = nullptr;
};
