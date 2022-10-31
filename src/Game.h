#pragma once

#include "Board.h"

enum class Player : bool {
	P1, P2
};

enum class GameState : uint8_t {
	IN_PROGRESS, TIE, P1_WON, P2_WON
};

class Game {
public:
	Game();
	explicit Game(Board&& initial_board);
	Game(const Game& other);
	~Game() = default;
	Game& operator=(const Game& other);

	void Play(Col col);

	void SwitchPlayer();

	Player Playing() { return m_playing; }

	[[nodiscard]] inline const Board& GetBoard() const { return m_board; }

	[[nodiscard]] inline bool IsGameOver() const { return GetState() != GameState::IN_PROGRESS; };

	[[nodiscard]] GameState GetState() const { return m_gameState; }

	[[nodiscard]] std::optional<std::pair<Player, Alignment>> Get4InARow() const;

	friend std::ostream& operator<<(std::ostream& out, const Game& game);

private:
	Board m_board;
	GameState m_gameState;
	Player m_playing;

	Col m_lastPlay = -1; // the last column to be played
	std::optional<Alignment> m_alignment{};

	void UpdateBoardState();
};