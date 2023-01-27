#pragma once

#include "MinMax.h"
#include "Board.h"
#include "GameEnums.h"

class Game {
public:
	// custom constructors
	Game();
	explicit Game(Board&& initial_board);

	~Game() = default;

	Game& Play(Col col);
	void Start(); // starts a thread that continuously checks if the algo needs to play
	void End(); // ends the thread in start

	void SwitchPlayer();
	void CreateAlgo(Player playAs, uint8_t depth);
	bool CanAcceptInput();
	void Reset();

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
	[[nodiscard]] inline const Score GetCurrentStaticScore() const { return m_CurrentStaticScore; }

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

	std::unique_ptr<MinMax> m_algoP1;
	std::unique_ptr<MinMax> m_algoP2;

	std::thread m_algoP1Thread;
	std::thread m_algoP2Thread;

	std::optional<Col> m_lastPlay; // the last column to be played, empty before first move
	std::optional<Alignment> m_alignment{};

	Score m_CurrentStaticScore = 0.f;

	void UpdateBoardState();
	void AlgoWorkerFunc(Player player);
	void AlgoPlayTurn(Player player);
};