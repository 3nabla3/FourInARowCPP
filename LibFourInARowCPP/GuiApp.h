#pragma once

#include "MinMax.h"
#include "Game.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


/// this class uses a vertex space of [0, 1] * [0, 1] while openGl uses [es [-1, 1] * [-1, 1]
class GuiApp {
public:
	explicit GuiApp(Game& game, uint16_t width = 700, uint16_t height = 700);
	void Run();
	~GuiApp();
private:
	static inline void Vertex(float x, float y);
	static void RenderCircle(float x, float y, float r);
	static void SetPieceColor(BoardPiece piece, bool highlight = false);
	static void KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] [[maybe_unused]] int scancode, int action,
							[[maybe_unused]] [[maybe_unused]] int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods);


	[[nodiscard]] std::optional<Col> ConvertGLFWposToColumn(double x) const;
	void RenderVerticalLines() const;
	void RenderHorizontalLines() const;
	void RenderGrid() const;
	void RenderPiece(BoardPiece piece, Row row, Col col) const;
	void ResetColor() const;
	void RenderBoard() const;

	void ProcessKey(uint8_t key);
	void Play(Col col);

	[[nodiscard]] inline bool ShouldHighlight(Row row, Col col) const;
	Game* m_Game;
	GLFWwindow* m_Window;

	float m_ResetColor[3] = {1.f, 1.f, 1.f};
	const uint16_t m_Height;
	const uint16_t m_Width;

	const float m_HeightMarginFrac = .03f;
	const float m_WidthMarginFrac = .03f;

	static constexpr int SCREEN_HZ = 144;
	static constexpr int s_TARGET_FPS = 20;
};