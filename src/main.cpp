#include "glog/logging.h"

#include "Board.h"
#include "MinMax.h"
#include "Game.h"
#include "GuiApp.h"

// static uint8_t GetColumnFromUser(const Board& board) {
// 	std::vector<Col> cols = board.GetValidColumns();
//
// 	while (true) {
// 		std::string inp;
// 		DLOG(INFO) << "Enter a column: ";
// 		std::cin >> inp;
// 		int col = stoi(inp);
//
// 		if (std::find(cols.begin(), cols.end(), col) != cols.end())
// 			return col;
// 		else {
// 			DLOG(WARNING) << "Column " << col << " is invalid";
// 		}
// 	}
// }

int main([[maybe_unused]] int argc, char** argv) {
	FLAGS_logtostdout = true;
	google::InitGoogleLogging(argv[0]);

	using BoardPiece::EMPTY;
	using BoardPiece::P1;
	using BoardPiece::P2;

	// BoardPiece initial[Board::N_COLS * Board::N_ROWS] = {
	// 		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
	// 		EMPTY,	EMPTY,	EMPTY,	P2,		EMPTY,	EMPTY,	EMPTY,
	// 		EMPTY,	EMPTY,	EMPTY,	P2,		EMPTY,	EMPTY,	EMPTY,
	// 		EMPTY,	EMPTY,	EMPTY,	P1,		EMPTY,	EMPTY,	EMPTY,
	// 		EMPTY,	EMPTY,	P1,		P1,		EMPTY,	EMPTY,	EMPTY,
	// 		P2,		P2,		P1,		P1,		EMPTY,	EMPTY,	P2,
	// };

	// Game game((Board(initial)));
	Game game;
	game.CreateAlgo(Player::P1, 3);
	game.CreateAlgo(Player::P2, 3);

	GuiApp gui(game);
	gui.Run();

	return 0;
}
