#pragma once

#include "Game.h"
#include "MinMax.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


/// this class uses a vertex space of [0, 1] * [0, 1] while openGl uses [-1, 1] * [-1, 1]
class GuiApp {
public:
	explicit GuiApp(Game  game, uint16_t width = 700, uint16_t height = 700);
	void AttachAlgo(MinMax algo);
	void Run();
private:
	static inline void Vertex(float x, float y);
	static void RenderCircle(float x, float y, float r);
	static void SetPieceColor(BoardPiece piece);
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
	void Play(Col col);

	[[nodiscard]] inline const Game& GetGame() const { return m_Game; }

	Game m_Game;
	GLFWwindow* m_Window;
	float m_ResetColor[3] = {1.f, 1.f, 1.f};
	// whether the algo was actually supplied at runtime,
	// or was it default constructed at compile time
	bool m_AlgoActive = false;
	MinMax m_Algo;

	const uint16_t m_Height;
	const uint16_t m_Width;
	const float m_HeightMarginFrac = .03f;
	const float m_WidthMarginFrac = .03f;

	static constexpr int SCREEN_HZ = 144;
	static constexpr int s_TARGET_FPS = 20;
};