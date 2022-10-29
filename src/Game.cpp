#include "pch.h"
#include "Game.h"
#include "glog/logging.h"


static Player GetPlaysNext(const Board& board) {
	// are there more of P1's or P2's pieces
	uint8_t balance = 0;
	for (int i = 0; i < Board::N_COLS * Board::N_ROWS; i++) {
		if (board.GetState()[i] == BoardPiece::P1)
			balance++;
		else if (board.GetState()[i] == BoardPiece::P2)
			balance--;
	}

	// otherwise the board is not valid
	assert(balance == 0 || balance == 1);

	if (balance == 0) // if each player has the same amount of pieces
		return Player::P1;
	else
		return Player::P2;
}

static BoardPiece ToPiece(Player p) {
	switch (p) {
		case Player::P1:
			return BoardPiece::P1;
		case Player::P2:
			return BoardPiece::P2;
		default: // only happens if p is uninitialized
			return BoardPiece::EMPTY;
	}
}

void Game::Play(Col col) {
	if (IsGameOver()) {
		LOG(WARNING) << "Game is over!";
		return;
	}
	m_board.InsertPiece(col, ToPiece(m_playing));
	SwitchPlayer();
	UpdateBoardState();
	m_lastPlay = col;

	if (m_gameState == GameState::TIE) LOG(INFO) << "Tie!";
	else if (m_gameState == GameState::P1_WON) LOG(INFO) << "Player 1 won!";
	else if (m_gameState == GameState::P2_WON) LOG(INFO) << "Player 2 won!";
}

Game::Game() : m_gameState(GameState::IN_PROGRESS), m_playing(Player::P1) {

}

Game::Game(Board initial_board) : m_board(initial_board), m_gameState(GameState::IN_PROGRESS) {

	m_playing = GetPlaysNext(initial_board);
}

void Game::UpdateBoardState() {
	// TODO: Update this function once the four in a row method has been
	//  implemented

	if (Get4InARow(Player::P1))
		m_gameState = GameState::P1_WON;
	else if (Get4InARow(Player::P2))
		m_gameState = GameState::P2_WON;

	else if (m_board.GetValidColumns().empty())  // if there are no more places to play
		m_gameState = GameState::TIE;
}

void Game::SwitchPlayer() {
	int temp = static_cast<int>(m_playing) + 1;
	m_playing = static_cast<Player>(temp % 2);
}

// check if the line has a 4 in a row
static int8_t GetIndex4InARow(const std::vector<BoardPiece>& line, Player player) {
	uint8_t count = 0;
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ToPiece(player)) {
			count++;
			if (count >= 4)
				return (int8_t) i;
		} else
			count = 0;
	}

	// if there was a 4 in a row, return the index
	return -1;
}

std::optional<std::array<Coord, 4>> Game::Get4InARow(Player player) const {

	// check the rows
	for (int row_i = 0; row_i < Board::N_ROWS; row_i++) {
		std::vector<BoardPiece> row = m_board.GetRow(row_i);

		// if there is an alignment in the column
		int8_t col_i = GetIndex4InARow(row, player);
		if (col_i > 0) {
			std::array<Coord, 4> array;
			for (int j = 0; j < 4; j++)
				array[j] = {row_i, row_i + j};
			return array;
		}
	}

	// check the columns
	for (int col_i = 0; col_i < Board::N_COLS; col_i++) {
		std::vector<BoardPiece> col = m_board.GetCol(col_i);

		// if there is an alignment in the column
		int8_t row_i = GetIndex4InARow(col, player);
		if (row_i > 0) {
			std::array<Coord, 4> array;
			for (int j = 0; j < 4; j++)
				array[j] = {row_i + j, col_i};
			return array;
		}
	}

	return std::nullopt;
}
