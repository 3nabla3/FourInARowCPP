#include "pch.h"
#include "Board.h"
#include "Game.h"

static uint8_t GetColumnFromUser(const Board& board) {
	auto cols = board.GetValidColumns();

	while (true) {
		std::string inp;
		LOG(INFO) << "Enter a column: ";
		std::cin >> inp;
		int col = stoi(inp);

		if (std::find(cols.begin(), cols.end(), col) != cols.end())
			return col;
		else {
			LOG(WARNING) << "Column " << col << " is invalid";
		}
	}
}

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	using BoardPiece::EMPTY;
	using BoardPiece::P1;
	using BoardPiece::P2;


	BoardPiece initial[] = {
			EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
			EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
			EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
			P1,		P1,		P2,		P2,		P1,		P1,		P2,
			P2,		P2,		P1,		P1,		P2,		P2,		P1,
			P1,		P1,		P2,		P2,		P1,		P1,		P2
	};

	Game game(( Board(initial) ));

	std::cout << game.GetBoard() << std::endl;
	while (!game.IsGameOver()) {
		game.Play(GetColumnFromUser(game.GetBoard()));
		std::cout << game.GetBoard() << std::endl;
	}

	return 0;
}
