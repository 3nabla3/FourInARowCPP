#include "pch.h"
#include "Board.h"


Board::Board(BoardPiece initial_state[]) {
	if (initial_state)
		Reset(initial_state);
	else
		Reset();
}


void Board::Reset() {
	for (auto& i: m_state)
		i = BoardPiece::EMPTY;
}


void Board::Reset(BoardPiece initial_state[]) {
	// TODO: should this copy? or just get the reference
	for (uint8_t i = 0; i < m_state.size(); i++)
		m_state[i] = initial_state[i];
}


std::vector<Col> Board::GetValidColumns() const {
	auto cols = std::vector<uint8_t>();

	for (Col col = 0; col < N_COLS; col++)
		if (GetPiece(0, col) == BoardPiece::EMPTY)
			cols.push_back(col);

	return std::move(cols);
}

bool Board::InsertPiece(Col col, BoardPiece piece) {
	if (GetPiece(0, col) != BoardPiece::EMPTY)
		return false; // the column is full

	Row row = 0; // start at the top

	// while the piece can still drop
	while (row < N_ROWS - 1 && GetPiece(row + 1, col) == BoardPiece::EMPTY)
		row++;

	SetPiece(row, col, piece);
	return true;
}

std::vector<BoardPiece> Board::GetRow(Row r) const {
	std::vector<BoardPiece> res(N_COLS);
	for (Col c = 0; c < N_COLS; c++)
		res[c] = GetPiece(r, c);

	return res;
}

std::vector<BoardPiece> Board::GetCol(Col c) const {
	std::vector<BoardPiece> res(N_ROWS);
	for (Row r = 0; r < N_ROWS; r++)
		res[r] = GetPiece(r, c);

	return res;
}

std::vector<BoardPiece> Board::GetUpDiag(uint8_t ud) const {
	return {};
}

std::vector<BoardPiece> Board::GetDnDiag(uint8_t dd) const {
	return {};
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	// print the column numbers
	out << "|\t";
	for (Col i = 0; i < Board::N_COLS; i++)
		out << std::to_string(i) << "\t";
	out << "|\n";

	// print the board
	for (Row row = 0; row < Board::N_ROWS; row++) {
		out << "|\t";
		for (Col col = 0; col < Board::N_COLS; col++) {
			// out << std::to_string(row * Board::N_COLS + col);
			switch (board.GetPiece(row, col)) {
				case (BoardPiece::EMPTY):
					out << " ";
					break;
				case (BoardPiece::P1):
					out << "X";
					break;
				case (BoardPiece::P2):
					out << "O";
					break;
			}
			out << "\t";
		}
		out << "|\n";
	}

	return out;
}




