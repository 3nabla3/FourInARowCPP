#pragma once

enum class Player : bool {
	P1, P2
};

enum class GameState : uint8_t {
	IN_PROGRESS, TIE, P1_WON, P2_WON
};
