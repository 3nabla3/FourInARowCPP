#pragma once
#include "Game.h"
#include "GLFW/glfw3.h"

/// this class uses a vertex space of [0, 1] * [0, 1] while openGl uses [-1, 1] * [-1, 1]
class Gui {
public:
	explicit Gui(const Game& game, uint16_t width = 700, uint16_t height = 700);
	void Run();
private:
	static inline void Vertex(float x, float y) ;
	void RenderVerticalLines() const;
	void RenderHorizontalLines() const;
	void RenderGrid() const;

	Game m_Game;
	GLFWwindow* m_Window;
	const uint16_t m_Width;
	const uint16_t m_Height;

	const float m_HeightMarginPercent = .03f;
	const float m_WidthMarginPercent = .03f;
};