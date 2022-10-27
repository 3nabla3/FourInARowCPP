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
	static constexpr char PLAYER_SYM[2] = {'X', 'O'};

	Game();
	explicit Game(Board initial_board);

	void Play(COL col);

	void SwitchPlayer();

	Player Playing();

	inline const Board& GetBoard() const { return m_board; }
	inline bool IsGameOver() { return m_gameState != GameState::IN_PROGRESS; };

	GameState GetState();

	std::array<int, 4> Get4InARow();

	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	Board m_board;
	GameState m_gameState;
	Player m_playing;

	COL m_lastPlay = -1; // the last column to be played

	void UpdateBoardState();
	static BoardPiece ToPiece(Player p);
};