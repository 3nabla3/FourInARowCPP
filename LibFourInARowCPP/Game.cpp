#include "pch.h"
#include "Game.h"
#include "glog/logging.h"

using std::optional, std::pair, std::vector, std::array;

Game::Game() : m_gameState(GameState::IN_PROGRESS), m_playing(Player::P1) {}

Game::Game(Board&& initial_board) : m_board(std::move(initial_board)), m_gameState(GameState::IN_PROGRESS) {
	DLOG(WARNING) << "Calling Game with R-value reference of Board";
	m_playing = GetPlaysNext(m_board);
	UpdateBoardState();
}

Game::Game(const Game& other)
		: m_algo(std::make_unique<MinMax>(other.m_algo->PlayingAs(), other.m_algo->GetMaxDepth())),
		  m_playing(other.m_playing), m_gameState(other.m_gameState), m_board(other.m_board),
		  m_algoActive(other.m_algoActive) {
	DLOG(WARNING) << "Calling Game copy constructor";
}

Game::Game(Game&& other) noexcept
		: m_algo(std::move(other.m_algo)), m_playing(other.m_playing), m_gameState(other.m_gameState),
		  m_board(std::move(other.m_board)),
		  m_algoActive(other.m_algoActive) {
	DLOG(WARNING) << "Calling Game R-value move constructor";
}

/// Construct a min max algorithm that the game object will own
void Game::CreateAlgo(Player playAs, uint8_t depth) {
	m_algo = std::make_unique<MinMax>(playAs, depth);
	m_algo->SetBoard(&m_board);
	m_algoActive = true;
}

/// Verify if the board is valid, and then who's turn is it to play
Player Game::GetPlaysNext(const Board& board) {

	// are there more of P1's or P2's pieces
	uint8_t balance = 0;
	for (int i = 0; i < Board::N_COLS * Board::N_ROWS; i++) {
		if (board.GetState()[i] == BoardPiece::P1)
			balance++;
		else if (board.GetState()[i] == BoardPiece::P2)
			balance--;
	}

	// otherwise the board is not valid
	LOG_IF(FATAL, balance != 0 && balance != 1) << "The board is invalid: balance is " << std::to_string(balance);

	if (balance == 0) // if each player has the same amount of pieces
		return Player::P1;
	else
		return Player::P2;
}

void Game::AlgoPlayTurn() {
	/// Carry out the steps the algorithm must play on its turn

	// if the human played before the algo,
	// shift the algo's tree to reflect that
	if (m_lastPlay) {
		m_algo->ShiftTree(m_lastPlay.value());
		m_algo->AddLayer();
	}

	uint32_t theoCount = TreeNode::NumNodesInFullTree(Board::N_COLS, m_algo->GetMaxDepth());
	DLOG(INFO) << "Number of theoretical nodes:\t" << theoCount;
	uint32_t actualCount = m_algo->GetHead()->GetNumNodes();
	DLOG(INFO) << "Number of actual nodes:\t" << actualCount;

	auto col = m_algo->GetBestMove();
	Play(col);
	m_algo->ShiftTree(col);
	m_algo->AddLayer();
}

void Game::AlgoWorkerFunc() {
	using namespace std::chrono_literals;
	// TODO: is is possible that the if condition is false?
	// if the tree has not been generated yet
	if (!m_algo->GetHead())
		m_algo->GenerateTree();

	while (m_algoActive) {
		if (m_playing == m_algo->PlayingAs() && !IsGameOver()) {
			AlgoPlayTurn();
		}
		// avoid overusing cpu
		std::this_thread::sleep_for(500ms);
	}
}

void Game::Start() {
	DLOG(INFO) << "Launching MinMax worker thread...";
	m_algoThread = std::thread(&Game::AlgoWorkerFunc, this);
}

void Game::End() {
	// ensures the thread stops
	m_algoActive = false;
	m_algoThread.join();
	DLOG(INFO) << "Algo thread has joined";
}

/// Let a user place a piece and update the state.
///  Return a reference to itself to allow method chaining
Game& Game::Play(Col col) {
	if (IsGameOver()) {
		LOG(ERROR) << "Game is over!";
		return *this;
	}

	// if the function returns false, the column is full,
	// so we cannot process the input
	if (!m_board.InsertPiece(col, ToPiece(m_playing)))
		return *this;

	SwitchPlayer();
	UpdateBoardState();
	m_lastPlay = col;

	if (m_gameState == GameState::TIE) LOG(INFO) << "Tie!";
	else if (m_gameState == GameState::P1_WON) LOG(INFO) << "Player 1 won!";
	else if (m_gameState == GameState::P2_WON) LOG(INFO) << "Player 2 won!";

	return *this;
}

void Game::UpdateBoardState() {
	// if there is an alignment
	if (auto alignment = Get4InARow(m_board)) {
		auto [player, coords] = alignment.value();
		if (player == Player::P1) m_gameState = GameState::P1_WON;
		else if (player == Player::P2) m_gameState = GameState::P2_WON;
		m_alignment = std::move(coords);  // remember the alignment to optimize the printing step

	} else if (m_board.GetValidColumns().empty())  // if there are no more places to play
		m_gameState = GameState::TIE;
}

GameState Game::AnalyzeGameState(const Board& board) {
	if (auto alignment = Get4InARow(board)) {
		auto [player, coords] = alignment.value();
		if (player == Player::P1) return GameState::P1_WON;
		if (player == Player::P2) return GameState::P2_WON;
	} else if (board.GetValidColumns().empty())
		return GameState::TIE;

	return GameState::IN_PROGRESS;
}

void Game::SwitchPlayer() {
	int temp = static_cast<int>(m_playing) + 1;
	m_playing = static_cast<Player>(temp % 2);
}

/// check if the line has a 4 in a row, and return the index if yes
static int8_t GetIndex4InARow(const std::vector<BoardPiece>& line, Player player) {
	uint8_t count = 0;
	BoardPiece player_piece = Game::ToPiece(player);
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

/// check if there is an alignment in the single row
static optional<Alignment> CheckSingleRow(const std::vector<BoardPiece>& row, Player player, Row row_i) {
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

/// check all row and all players
static optional<pair<Player, Alignment>> CheckRows(const Board& board) {
	Player players[] = {Player::P1, Player::P2};

	for (int row_i = 0; row_i < Board::N_ROWS; row_i++) {
		std::vector<BoardPiece> row = board.GetRow(row_i);
		for (auto player: players) {
			auto align = CheckSingleRow(row, player, row_i);
			if (align)
				return pair(player, align.value());
		}
	}

	return {};
}

/// check if there is an alignment in the single column
static optional<Alignment> CheckSingleCol(const vector<BoardPiece>& col, Player player, Col col_i) {
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

/// check all columns
static optional<pair<Player, Alignment>> CheckColumns(const Board& board) {
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
			y = up_diag_i - idx;
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

/// check all up diags
static optional<pair<Player, Alignment>> CheckUpDiag(const Board& board) {
	Player players[] = {Player::P1, Player::P2};

	for (int up_diag_i = 0; up_diag_i < Board::N_COLS + Board::N_ROWS - 1; up_diag_i++) {
		std::vector<BoardPiece> diag = board.GetUpDiag(up_diag_i);
		for (auto player: players) {
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

/// check all down diags
static optional<pair<Player, Alignment>> CheckDnDiag(const Board& board) {
	Player players[] = {Player::P1, Player::P2};

	for (int dn_diag_i = 0; dn_diag_i < Board::N_COLS + Board::N_ROWS - 1; dn_diag_i++) {
		std::vector<BoardPiece> diag = board.GetDnDiag(dn_diag_i);
		for (auto player: players) {
			auto align = CheckSingleDnDiag(diag, player, dn_diag_i);
			if (align)
				return pair(player, align.value());
		}
	}

	return {};
}

optional<pair<Player, Alignment>> Game::Get4InARow(const Board& board) {
	// check the rows
	if (auto alignment = CheckRows(board)) {
		// DLOG(INFO) << "Found alignment in row";
		return alignment;
	}

	// check the columns
	if (auto alignment = CheckColumns(board)) {
		// DLOG(INFO) << "Found alignment in column";
		return alignment;
	}

	// check up diag
	if (auto alignment = CheckUpDiag(board)) {
		// DLOG(INFO) << "Found alignment in up diag";
		return alignment;
	}

	// check down diag
	if (auto alignment = CheckDnDiag(board)) {
		// DLOG(INFO) << "Found alignment in down diag";
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
