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

BoardPiece Game::ToPiece(Player p) {
	switch (p) {
		case Player::P1:
			return BoardPiece::P1;
		case Player::P2:
			return BoardPiece::P2;
	}
}

void Game::Play(COL col) {
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

Game::Game()
		: m_gameState(GameState::IN_PROGRESS), m_playing(Player::P1) {

}

Game::Game(Board initial_board)
		: m_board(initial_board), m_gameState(GameState::IN_PROGRESS) {

	m_playing = GetPlaysNext(initial_board);
}

void Game::UpdateBoardState() {
	// TODO: Update this function once the four in a row method has been
	//  implemented

	m_gameState = GameState::IN_PROGRESS;
}

void Game::SwitchPlayer() {
	int temp = static_cast<int>(m_playing) + 1;
	m_playing = static_cast<Player>(temp % 2);
}