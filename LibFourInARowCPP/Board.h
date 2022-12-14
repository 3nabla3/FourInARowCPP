#pragma once

enum class BoardPiece : int8_t {
	EMPTY = -1, P1, P2
};
std::ostream& operator<<(std::ostream& out, BoardPiece piece);

typedef uint8_t Col;
typedef uint8_t Row;
typedef std::pair<Row, Col> Coord;

// a 4 in a row is an array of size 4
typedef std::array<Coord, 4> Alignment;

class Board {
public:
	const static Col N_COLS = 7;
	const static Row N_ROWS = 6;
	static constexpr uint8_t GetSize() { return N_COLS * N_ROWS; }

	typedef std::array<BoardPiece, Board::N_COLS * Board::N_ROWS> BoardState;

	// Custom constructor
	explicit Board(const BoardPiece initial_state[] = nullptr);

	// Rule of 5
	Board(const Board& other);
	Board(Board&& other) noexcept;
	Board& operator=(const Board& other);
	Board& operator=(Board&& other) noexcept;
	~Board() = default;

	void Reset();
	void Reset(const BoardPiece initial_state[]);
	bool InsertPiece(Col col, BoardPiece piece);

	[[nodiscard]]
	std::vector<Col> GetValidColumns() const;

	[[nodiscard]]
	inline const BoardState& GetState() const { return m_state; }

	[[nodiscard]]
	inline BoardPiece GetPiece(Row row, Col col) const {
		return m_state[row * N_COLS + col];
	}

	[[nodiscard]] std::vector<BoardPiece> GetRow(Row r) const;
	[[nodiscard]] std::vector<BoardPiece> GetCol(Col c) const;
	[[nodiscard]] std::vector<BoardPiece> GetUpDiag(uint8_t ud) const;
	[[nodiscard]] std::vector<BoardPiece> GetDnDiag(uint8_t dd) const;

	friend std::ostream& operator<<(std::ostream& out, const Board& board);
	[[nodiscard]] std::string ToStringWithAlignment(const Alignment& align) const;

private:
	BoardState m_state{};

	inline void SetPiece(Row row, Col col, BoardPiece piece) {
		m_state[row * N_COLS + col] = piece;
	}
};
