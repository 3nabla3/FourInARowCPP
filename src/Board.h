#pragma once


enum class BoardPiece : int8_t {
	EMPTY = -1, P1, P2
};

typedef uint8_t COL;
typedef uint8_t ROW;

class Board {
public:
	const static COL N_COLS = 7;
	const static ROW N_ROWS = 6;

	explicit Board(BoardPiece initial_state[] = nullptr);

	void Reset();
	void Reset(BoardPiece initial_state[]);
	bool InsertPiece(COL col, BoardPiece piece);

	[[nodiscard]]
	std::unique_ptr<std::vector<COL>> GetValidColumns() const;

	[[nodiscard]]
	inline const BoardPiece* GetState() const { return m_state; }

	[[nodiscard]]
	inline BoardPiece GetPiece(ROW row, COL col) const {
		return m_state[row * N_COLS + col];
	}

	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	BoardPiece m_state[N_COLS * N_ROWS]{};

	inline void SetPiece(ROW row, COL col, BoardPiece piece) {
		m_state[row * N_COLS + col] = piece;
	}
};
