#pragma once


enum class BoardPiece : int8_t {
	EMPTY = -1, P1, P2
};

typedef uint8_t Col;
typedef uint8_t Row;
typedef std::pair<Row, Col> Coord;

class Board {
public:
	const static Col N_COLS = 7;
	const static Row N_ROWS = 6;

	explicit Board(BoardPiece initial_state[] = nullptr);

	void Reset();
	void Reset(BoardPiece initial_state[]);
	bool InsertPiece(Col col, BoardPiece piece);

	[[nodiscard]]
	std::unique_ptr<std::vector<Col>> GetValidColumns() const;

	[[nodiscard]]
	inline const BoardPiece* GetState() const { return m_state; }

	[[nodiscard]]
	inline BoardPiece GetPiece(Row row, Col col) const {
		return m_state[row * N_COLS + col];
	}

	[[nodiscard]] std::vector<BoardPiece> GetRow(Row r) const;
	[[nodiscard]] std::vector<BoardPiece> GetCol(Col c) const;
	[[nodiscard]] std::vector<BoardPiece> GetUpDiag(uint8_t ud) const;
	[[nodiscard]] std::vector<BoardPiece> GetDnDiag(uint8_t dd) const;

	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	BoardPiece m_state[N_COLS * N_ROWS]{};

	inline void SetPiece(Row row, Col col, BoardPiece piece) {
		m_state[row * N_COLS + col] = piece;
	}
};
