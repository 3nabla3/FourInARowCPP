#include "pch.h"
#include "Board.h"
#include "Game.h"
#include "Gui.h"

static uint8_t GetColumnFromUser(const Board& board) {
	std::vector<Col> cols = board.GetValidColumns();

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

	Game game;
	game
	.Play(3)
	.Play(3)
	.Play(4)
	.Play(5);

	Gui gui(std::move(game));
	gui.Run();

	return 0;
}
