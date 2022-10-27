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

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	Game game;

	while (!game.IsGameOver()) {
		game.Play(GetColumnFromUser(game.GetBoard()));
		std::cout << game.GetBoard() << std::endl;
	}

	return 0;
}
