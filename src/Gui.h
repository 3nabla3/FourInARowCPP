#pragma once
#include "Game.h"
#include "GLFW/glfw3.h"

/// this class uses a vertex space of [0, 1] * [0, 1] while openGl uses [-1, 1] * [-1, 1]
class Gui {
public:
	explicit Gui(Game game, uint16_t width = 700, uint16_t height = 700);
	void Run();
private:
	static inline void Vertex(float x, float y) ;
	static void RenderCircle(float x, float y, float r) ;
	static void SetPieceColor(BoardPiece piece);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


	void RenderVerticalLines() const;
	void RenderHorizontalLines() const;
	void RenderGrid() const;
	void RenderPiece(BoardPiece piece, Row row, Col col) const;
	void ResetColor() const;
	void RenderBoard() const;

	Game m_Game;
	GLFWwindow* m_Window;
	float m_ResetColor[3] = {1.f, 1.f, 1.f};

	const uint16_t m_Height;
	const uint16_t m_Width;
	const float m_HeightMarginFrac = .03f;
	const float m_WidthMarginFrac = .03f;

	static constexpr int SCREEN_HZ = 144;
	static const int s_TARGET_FPS = 20;

};