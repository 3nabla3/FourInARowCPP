#pragma once

#include "MinMax.h"
#include "Game.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


/// this class uses a vertex space of [0, 1] * [0, 1] while openGl uses [es [-1, 1] * [-1, 1]
class GuiApp {
public:
	explicit GuiApp(Game& game, uint16_t width = 825, uint16_t height = 750);
	void Run();
	~GuiApp();
private:
	static inline void Vertex(float x, float y);
	static void RenderRectangle(float x1, float y1, float x2, float y2, const float* color);
	static void RenderCircle(float x, float y, float r);
	static void KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] [[maybe_unused]] int scancode, int action,
							[[maybe_unused]] [[maybe_unused]] int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods);


	[[nodiscard]] std::optional<Col> ConvertGLFWposToColumn(double x) const;
	void RenderEvalVar(float score) const;
	void RenderVerticalLines() const;
	void RenderHorizontalLines() const;
	void RenderGrid() const;
	void RenderPiece(BoardPiece piece, Row row, Col col) const;
	void ResetColor() const;
	void RenderBoard() const;
	void SetPieceColor(BoardPiece piece, bool highlight = false) const;

	void ProcessKey(uint8_t key);
	void Play(Col col);

	void UpdateEvalBar() const;

	[[nodiscard]] inline bool ShouldHighlight(Row row, Col col) const;
	Game* m_Game;
	GLFWwindow* m_Window;

	const float m_ResetColor[3] = {1.f, 1.f, 1.f};

	static constexpr float m_Black[3] = {0.f, 0.f, 0.f};
	static constexpr float m_White[3] = {1.f, 1.f, 1.f};

	const float m_P1ColorHighlight[3] = {1.f, 0.f, 0.f};
	const float m_P2ColorHighlight[3] = {0.f, 0.f, 1.f};
	const float m_P1Color[3] = {.5f, 0.f, 0.f};
	const float m_P2Color[3] = {0.f, 0.f, .5f};

	const uint16_t m_Height;
	const uint16_t m_Width;

	const float m_HeightMarginFrac = .03f;
	const float m_WidthMarginFrac = .10f;

	static constexpr int SCREEN_HZ = 144;
	static constexpr int s_TARGET_FPS = 20;
};