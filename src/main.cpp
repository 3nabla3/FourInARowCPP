#include <iostream>
#include "glog/logging.h"

#include "Board.h"
#include "MinMax.h"
#include "Game.h"
#include "GuiApp.h"

[[maybe_unused]] static uint8_t GetColumnFromUser(const Board& board) {
	std::vector<Col> cols = board.GetValidColumns();

	while (true) {
		std::string inp;
		DLOG(INFO) << "Enter a column: ";
		std::cin >> inp;
		int col = stoi(inp);

		if (std::find(cols.begin(), cols.end(), col) != cols.end())
			return col;
		else {
			DLOG(WARNING) << "Column " << col << " is invalid";
		}
	}
}

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	Game game;
	MinMax mm(Player::P2, 5);
	game.AttachAlgo(mm);

	GuiApp gui(game);
	gui.Run();

	return 0;
}
