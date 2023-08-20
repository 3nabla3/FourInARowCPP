#include "glog/logging.h"

#include "Board.h"
#include "MinMax.h"
#include "Game.h"
#include "GuiApp.h"

static BoardPiece* ArrayFromString(const std::string& initialString) {
	auto* array = new BoardPiece[Board::N_COLS * Board::N_ROWS];
	for (int i = 0; i < Board::N_COLS * Board::N_ROWS; i++)
		array[i] = BoardPiece::EMPTY;


	int idx = 0;
	for (const char c : initialString) {
		if (c == '\t' || c == '\n') idx++;
		else if (c == 'x') array[idx] = BoardPiece::P1;
		else if (c == 'o') array[idx] = BoardPiece::P2;
		else
			LOG(FATAL) << "Invalid character in string";
	}

	return array;
}

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	Game game;
//	game.CreateAlgo(Player::P1, 7);
	game.CreateAlgo(Player::P2, 7);

	GuiApp(game).Run();
	return 0;
}
