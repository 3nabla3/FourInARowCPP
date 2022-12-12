#pragma once

#include "MinMax.h"
#include "Board.h"
#include "GameEnums.h"

class Game {
public:
	// custom constructors
	Game();
	explicit Game(Board&& initial_board);

	// Rule of 5
	Game(const Game& other);
	Game(Game&& other) noexcept;
	~Game() = default;

	Game& Play(Col col);
	void Start(); // starts a thread that continuously checks if the algo needs to play
	void End(); // ends the thread in start

	void SwitchPlayer();
	void CreateAlgo(Player playAs, uint8_t depth);

	[[nodiscard]] Player Playing() const { return m_playing; }

	[[nodiscard]] inline bool IsGameOver() const { return GetState() != GameState::IN_PROGRESS; };

	inline Board* GetBoardPtr() { return &m_board; }
	[[nodiscard]] inline const Board& GetBoard() const { return m_board; }

	[[nodiscard]] GameState GetState() const { return m_gameState; }
	static GameState AnalyzeGameState(const Board& board);

	[[nodiscard]] static std::optional<std::pair<Player, Alignment>> Get4InARow(const Board& board);

	[[nodiscard]] inline const std::optional<Alignment>& GetAlignment() const { return m_alignment; }

	[[nodiscard]] inline std::optional<Col> GetLastPlay() const { return m_lastPlay; }

	static Player GetPlaysNext(const Board& board);

	/// converts a player to its corresponding piece
	static inline BoardPiece ToPiece(Player p) {
		if (p == Player::P1) return BoardPiece::P1;
		return BoardPiece::P2;
	}

	friend std::ostream& operator<<(std::ostream& out, const Game& game);
private:
	Board m_board;
	GameState m_gameState;
	Player m_playing;

	// used to know whether the algo was default constructed 
	// at compile time or attached by the user
	bool m_algoActive = false;
	std::unique_ptr<MinMax> m_algo;
	std::thread m_algoThread;

	std::optional<Col> m_lastPlay; // the last column to be played, empty before first move
	std::optional<Alignment> m_alignment{};

	void UpdateBoardState();
	void AlgoWorkerFunc();
	void AlgoPlayTurn();
};