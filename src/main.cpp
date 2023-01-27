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

//	std::string initialString = "\t\t\t\t\t\t\n"
//								"\t\t\t\t\t\t\n"
//								"\t\to\t\to\t\t\n"
//								"\to\to\t\tx\t\t\n"
//								"\tx\tx\t\tx\t\t\n"
//								"\to\to\t\tx\tx\t";
//	std::unique_ptr<BoardPiece[]> initial(ArrayFromString(initialString));

//	Game game((Board(initial.get())));
	Game game;
	// game.CreateAlgo(Player::P1, 8);
	game.CreateAlgo(Player::P2, 4);

	GuiApp gui(game);
	gui.Run();

	return 0;
}
