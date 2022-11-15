#pragma once

#include "MinMax.h"
#include "Board.h"
#include "Player.h"

enum class GameState : uint8_t {
	IN_PROGRESS, TIE, P1_WON, P2_WON
};

class Game {
public:
	// custom constructors
	Game();
	explicit Game(Board&& initial_board);

	// Rule of 5
	Game(const Game& other);
	Game(Game&& other) noexcept;
	Game& operator=(const Game& other);
	Game& operator=(Game&& other) noexcept;
	~Game() = default;

	Game& Play(Col col);
	void Start(); // starts a thread that continuously checks if the algo needs to play
	void End(); // ends the thread in start

	void SwitchPlayer();
	void AttachAlgo(MinMax mm);

	Player Playing() { return m_playing; }

	[[nodiscard]] inline bool IsGameOver() const { return GetState() != GameState::IN_PROGRESS; };

	[[nodiscard]] inline const Board& GetBoard() const { return m_board; }
	[[nodiscard]] inline Board* GetBoardPtr() { return &m_board; }
	[[nodiscard]] GameState GetState() const { return m_gameState; }
	[[nodiscard]] std::optional<std::pair<Player, Alignment>> Get4InARow() const;
	[[nodiscard]] inline const std::optional<Alignment>& GetAlignment() const { return m_alignment; }
	[[nodiscard]] inline Col GetLastPlay() const { return m_lastPlay; }

	friend std::ostream& operator<<(std::ostream& out, const Game& game);

	void Func() {
		if (m_algoActive && m_playing == m_algo.PlayingAs() && !IsGameOver()) {
			Play(m_algo.GetBestMove());
		}
	}
private:
	Board m_board;
	GameState m_gameState;
	Player m_playing;

	// used to know whether the algo was default constructed 
	// at compile time or attached by the user
	bool m_algoActive = false;
	MinMax m_algo;
	std::thread m_algoThread;

	Col m_lastPlay = -1; // the last column to be played
	std::optional<Alignment> m_alignment{};

	Player GetPlaysNext();
	void UpdateBoardState();
	void AlgoWorkerFunc();
};