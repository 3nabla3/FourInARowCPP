#include "pch.h"
#include "Board.h"
#include "Game.h"

static uint8_t GetColumnFromUser(const Board& board) {
	auto cols = board.GetValidColumns();

	while (true) {
		int col;
		std::cout << "Enter a column: " << std::endl;
		std::cin >> col;
		std::cout << "You entered " << col << std::endl;

		if (std::find(cols->begin(), cols->end(), col) != cols->end())
			return col;
		else {
			LOG(WARNING) << "Column " << col << " is invalid";
		}
	}
}

static void SwitchPlayer(BoardPiece& player) {
	auto temp = static_cast<int>(player) + 1;
	player = static_cast<BoardPiece>(temp % 2);
}

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	Game game;
	for (int i = 0; i < 10; i++) {
		game.Play(GetColumnFromUser(game.GetBoard()));
		std::cout << game.GetBoard() << std::endl;
	}

	return 0;
}
