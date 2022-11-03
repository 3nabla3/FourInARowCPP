#include "pch.h"
#include "Game.h"
#include "glog/logging.h"

using std::optional, std::pair, std::vector, std::array;

static Player GetPlaysNext(const Board& board) {
	/// Verify if the board is valid, and then who's turn is it to play

	// are there more of P1's or P2's pieces
	uint8_t balance = 0;
	for (int i = 0; i < Board::N_COLS * Board::N_ROWS; i++) {
		if (board.GetState()[i] == BoardPiece::P1)
			balance++;
		else if (board.GetState()[i] == BoardPiece::P2)
			balance--;
	}

	// otherwise the board is not valid
	LOG_IF(FATAL, balance != 0 && balance != 1) << "The board is not balanced: " << std::to_string(balance);

	if (balance == 0) // if each player has the same amount of pieces
		return Player::P1;
	else
		return Player::P2;
}

static BoardPiece ToPiece(Player p) {
	/// converts a player to its corresponding piece
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
	/// let a user place a piece and update the state

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

Game::Game() : m_gameState(GameState::IN_PROGRESS), m_playing(Player::P1) {}

Game::Game(Board&& initial_board) : m_board(initial_board), m_gameState(GameState::IN_PROGRESS) {
	m_playing = GetPlaysNext(initial_board);
	UpdateBoardState();
}

Game::Game(const Game& other) {
	m_playing = other.m_playing;
	m_gameState = other.m_gameState;
	m_board = other.m_board;
}

Game& Game::operator=(const Game& other) {
	m_playing = other.m_playing;
	m_gameState = other.m_gameState;
	m_board = other.m_board;
	return *this;
}

void Game::UpdateBoardState() {
	// if there is an alignment
	if (auto alignment = Get4InARow()) {
		auto [player, coords] = alignment.value();
		if (player == Player::P1) m_gameState = GameState::P1_WON;
		else if (player == Player::P2) m_gameState = GameState::P2_WON;
		m_alignment = coords;  // remember the alignment to optimize the printing step

	} else if (m_board.GetValidColumns().empty())  // if there are no more places to play
		m_gameState = GameState::TIE;
}

void Game::SwitchPlayer() {
	int temp = static_cast<int>(m_playing) + 1;
	m_playing = static_cast<Player>(temp % 2);
}

static int8_t GetIndex4InARow(const std::vector<BoardPiece>& line, Player player) {
	/// check if the line has a 4 in a row, and return the index if yes

	uint8_t count = 0;
	BoardPiece player_piece = ToPiece(player);
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == player_piece) {
			count++;
			if (count == 4)  // if there is a 4 in a row
				return (int8_t) (i - 3);  // jump back 3 to get the start of the line
		} else
			count = 0;  // Reset the count if we broke the streak
	}
	// since there was no four in a row
	return -1;
}

static optional<Alignment> CheckSingleRow(const std::vector<BoardPiece>& row, Player player, Row row_i) {
	/// check if there is an alignment in the single row

	// if there is an alignment in the column
	int8_t idx = GetIndex4InARow(row, player);
	if (idx >= 0) {
		Alignment array;
		for (int j = 0; j < 4; j++)
			array[j] = {row_i, idx + j};
		return array;
	}
	return {};
}

static optional<pair<Player, Alignment>> CheckRows(const Board& board) {
	/// check all row and all players

	Player players[] = {Player::P1, Player::P2};

	for (int row_i = 0; row_i < Board::N_ROWS; row_i++) {
		std::vector<BoardPiece> row = board.GetRow(row_i);
		for (auto player : players) {
			auto align = CheckSingleRow(row, player, row_i);
			if (align)
				return pair(player, align.value());
		}
	}

	return {};
}

static optional<Alignment> CheckSingleCol(const vector<BoardPiece>& col, Player player, Col col_i) {
	/// check if there is an alignment in the single column

	// if there is an alignment in the column
	int8_t idx = GetIndex4InARow(col, player);
	if (idx >= 0) {
		Alignment array;
		for (int j = 0; j < 4; j++)
			array[j] = {idx + j, col_i};
		return array;
	}

	return {};
}

static optional<pair<Player, Alignment>> CheckColumns(const Board& board) {
	/// check all columns

	Player players[] = {Player::P1, Player::P2};

	for (int col_i = 0; col_i < Board::N_COLS; col_i++) {
		std::vector<BoardPiece> col = board.GetCol(col_i);
		for (auto player: players) {
			auto align = CheckSingleCol(col, player, col_i);
			if (align)
				return pair(player, align.value());
		}
	}
	return {};
}

static optional<Alignment> CheckSingleUpDiag(const vector<BoardPiece>& diag, Player player, uint8_t up_diag_i) {
	int8_t idx = GetIndex4InARow(diag, player);
	if (idx >= 0) {
		Alignment array;
		uint8_t x, y;
		if (up_diag_i < Board::N_ROWS) {
			y = up_diag_i - 1;
			x = idx;
		} else {
			y = Board::N_ROWS - idx - 1;
			x = up_diag_i - Board::N_ROWS + idx + 1;
		}
		for (int j = 0; j < 4; j++)
			array[j] = {y - j, x + j};
		return array;
	}

	return {};
}

static optional<pair<Player, Alignment>> CheckUpDiag(const Board& board) {
	/// check all up diags

	Player players[] = {Player::P1, Player::P2};

	for (int up_diag_i = 0; up_diag_i < Board::N_COLS + Board::N_ROWS - 1; up_diag_i++) {
		std::vector<BoardPiece> diag = board.GetUpDiag(up_diag_i);
		for (auto player : players) {
			auto align = CheckSingleUpDiag(diag, player, up_diag_i);
			if (align)
				return pair(player, align.value());
		}
	}

	return {};
}

static optional<Alignment> CheckSingleDnDiag(const vector<BoardPiece>& diag, Player player, uint8_t dn_diag_i) {
	// if there is an alignment in the column
	int8_t idx = GetIndex4InARow(diag, player);
	if (idx >= 0) {
		Alignment array;
		uint8_t x, y;
		if (dn_diag_i < Board::N_ROWS) {
			y = Board::N_ROWS - dn_diag_i + idx - 1;
			x = idx;
		} else {
			y = idx;
			x = dn_diag_i - Board::N_ROWS + idx + 1;
		}
		for (int j = 0; j < 4; j++)
			array[j] = {y + j, x + j};
		return array;
	}

	return {};
}

static optional<pair<Player, Alignment>> CheckDnDiag(const Board& board) {
	/// check all down diags

	Player players[] = {Player::P1, Player::P2};

	for (int dn_diag_i = 0; dn_diag_i < Board::N_COLS + Board::N_ROWS - 1; dn_diag_i++) {
		std::vector<BoardPiece> diag = board.GetDnDiag(dn_diag_i);
		for (auto player : players) {
			auto align = CheckSingleDnDiag(diag, player, dn_diag_i);
			if (align)
				return pair(player, align.value());
		}
	}

	return {};
}

optional<pair<Player, Alignment>> Game::Get4InARow() const {
	// check the rows
	if (auto alignment = CheckRows(m_board)) {
		LOG(INFO) << "Found alignment in row";
		return alignment;
	}

	// check the columns
	if (auto alignment = CheckColumns(m_board)) {
		LOG(INFO) << "Found alignment in column";
		return alignment;
	}

	// check up diag
	if (auto alignment = CheckUpDiag(m_board)) {
		LOG(INFO) << "Found alignment in up diag";
		return alignment;
	}

	// check down diag
	if (auto alignment = CheckDnDiag(m_board)) {
		LOG(INFO) << "Found alignment in down diag";
		return alignment;
	}

	return {};
}

std::ostream& operator<<(std::ostream& out, const Game& game) {
	if (game.m_alignment) {
		auto coords = game.m_alignment.value();
		out << game.GetBoard().ToStringWithAlignment(coords);
	} else
		out << game.GetBoard();

	return out;
}
