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
	for (int i = 0; i < m_state.size(); i++)
		m_state[i] = initial_state[i];
}


std::vector<Col> Board::GetValidColumns() const {
	auto cols = std::vector<uint8_t>();

	for (Col col = 0; col < N_COLS; col++)
		if (GetPiece(0, col) == BoardPiece::EMPTY)
			cols.push_back(col);

	return cols;
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

std::vector<BoardPiece> Board::GetUpDiag(uint8_t diag_i) const {
	int8_t row_i, col_i;
	auto i = static_cast<int8_t>(diag_i);

	if (diag_i < Board::N_ROWS) {
		row_i = i;
		col_i = 0;
	} else {
		row_i = Board::N_ROWS - 1;
		col_i = static_cast<int8_t>(i - Board::N_ROWS + 1);
	}
	// TODO: Find the size of the generated diag to optimize
	std::vector<BoardPiece> res;
	while (row_i >= 0 && col_i < Board::N_COLS) {
		res.push_back(GetPiece(row_i, col_i));
		row_i--;
		col_i++;
	}

	return res;
}

std::vector<BoardPiece> Board::GetDnDiag(uint8_t diag_i) const {
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
		for (Col col = 0; col < Board::N_COLS; col++)
			out << board.GetPiece(row, col) << "\t";
		out << "|\n";
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, BoardPiece piece) {
	switch(piece) {
		case (BoardPiece::EMPTY):
			out << ".";
			break;
		case (BoardPiece::P1):
			out << "X";
			break;
		case (BoardPiece::P2):
			out << "O";
			break;
	}
	return out;
}

