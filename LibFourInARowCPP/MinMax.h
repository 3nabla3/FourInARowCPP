#pragma once

#include "Board.h"
#include "GameEnums.h"
#include "TreeNode.h"

class MinMax {
public:
	explicit MinMax(Player playingAs = Player::P2, uint8_t depth = 5, Board* gameBoard = nullptr);

	~MinMax() = default;

	void SetBoard(Board* board) { m_Board = board; }
	void GenerateTree();
	void ShiftTree(Col col);
	void AddLayer(int count=1);

	[[nodiscard]] inline Player PlayingAs() const { return m_playingAs; }
	[[nodiscard]] inline uint8_t GetMaxDepth() const {return m_maxDepth; }
	[[nodiscard]] const std::shared_ptr<TreeNode>& GetHead() const { return m_head; }
	[[nodiscard]] Col GetBestMove() const;
private:
	Player m_playingAs;
	uint8_t m_maxDepth;

	// pointer to the running game's board
	const Board* m_Board = nullptr;
	std::shared_ptr<TreeNode> m_head;

	static int ChoseRandomChild(const TreeNode** children, int size);
};
