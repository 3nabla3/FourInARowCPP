#pragma once

#include "cstdint"
#include "Board.h"
#include "GameEnums.h"

typedef int8_t Score;

class TreeNode {
public:
	TreeNode(Board board, bool isMaximizing, Col delta = -1);
	~TreeNode() = default;

	static Score AnalyzeLine(const std::vector<BoardPiece>& line, Player player);

	[[nodiscard]] inline const std::vector<std::shared_ptr<TreeNode>>& GetChildren() const { return m_Children; }

	[[nodiscard]] inline const Board& GetBoard() const { return m_Board; }

	[[nodiscard]] inline bool IsMaximizing() const { return m_IsMaximizing; }

	[[nodiscard]] inline Col GetDelta() const { return m_delta; }

	[[nodiscard]] Score GetScore() const;
	[[nodiscard]] GameState GetGameState() const;
	[[nodiscard]] uint32_t GetNumNodes() const;


	[[nodiscard]] inline Player GetPlaysNext() const { return m_IsMaximizing ? Player::P1 : Player::P2; }
	[[nodiscard]] std::shared_ptr<TreeNode>& GetChildFromDelta(uint8_t index);

	void GenerateTree(uint8_t depth);
	void AddLayer();

	static uint32_t NumNodesInFullTree(int nPerLayer, int depth);
private:
	const Board m_Board;
	const bool m_IsMaximizing;
	std::vector<std::shared_ptr<TreeNode>> m_Children;

	// Act as a cache for their respective "Get" methods
	// Should be reset appropriately to allow recalculation
	mutable std::optional<Score> m_Score;
	mutable std::optional<GameState> m_gameState;

	Col m_delta; // the column that was played on the parent board that led to this board


	[[nodiscard]] bool IsLeaf() const { return m_Children.empty(); }

	[[nodiscard]] Score MinMax(Score alpha, Score beta) const;
	[[nodiscard]] Score CalculateStaticScore() const;
	[[nodiscard]] Score CalculateStaticPlayerScore(Player player) const;
	static Score AnalyzeLine_Impl(std::vector<BoardPiece>::const_iterator begin,
								  std::vector<BoardPiece>::const_iterator end, Player player);
	static inline bool FourInARowImpossible(std::vector<BoardPiece>::const_iterator begin,
											std::vector<BoardPiece>::const_iterator end, Player player);
};
