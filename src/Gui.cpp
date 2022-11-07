#include "Gui.h"
#include "glog/logging.h"
#include "Timer.h"

Gui::Gui(Game game, uint16_t width, uint16_t height)
		: m_Game(std::move(game)), m_Width(width), m_Height(height) {
	LOG(INFO) << "Attempting to initialize window, please wait...";
	if (!glfwInit())
		LOG(FATAL) << "Could not initialize glfw!";
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", nullptr, nullptr);
	if (!m_Window) {
		glfwTerminate();
		LOG(FATAL) << "Could not create window!";
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG(FATAL) << "Failed to initialize GLAD";
	}

	LOG(INFO) << "Completed windows initialization!";

	glfwSetKeyCallback(m_Window, KeyCallback);
}

void Gui::Run() {
	using namespace std::literals::chrono_literals;

	LOG(INFO) << "Starting main loop";
	while (!glfwWindowShouldClose(m_Window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		RenderGrid();
		RenderBoard();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
		glfwSwapInterval(SCREEN_HZ / s_TARGET_FPS);
	}
	LOG(INFO) << "Main loop ended";
}

void Gui::Vertex(float x, float y) {
	/// converts from a [0, 1] * [0, 1] space to a [-1, 1] * [-1, 1] space
	glVertex2f(x * 2 - 1, y * 2 - 1);
}

void Gui::RenderGrid() const {
	glBegin(GL_LINES);
	RenderVerticalLines();
	RenderHorizontalLines();
	glEnd();
}

void Gui::RenderVerticalLines() const {
	float startX = m_WidthMarginFrac;
	float endX = 1 - startX;
	float stepX = (endX - startX) / Board::N_COLS;

	// for some reason, clang-tidy doesn't like for loop with floats
	float x = startX;
	while (x < endX) {
		Vertex(x, m_HeightMarginFrac);
		Vertex(x, 1 - m_HeightMarginFrac);
		x += stepX;
	}
}

void Gui::RenderHorizontalLines() const {
	float startY = m_HeightMarginFrac;
	float endY = 1 - startY;
	float stepY = (endY - startY) / Board::N_ROWS;

	float y = startY;
	while (y < endY) {
		Vertex(m_WidthMarginFrac, y);
		Vertex(1 - m_WidthMarginFrac, y);
		y += stepY;
	}
}

void Gui::RenderPiece(BoardPiece piece, Row row, Col col) const {
	// flip the row_i because the 0th row is at the top in memory but
	// should be at the bottom in the grid
	row = Board::N_ROWS - 1 - row;

	float stepX = (1 - m_WidthMarginFrac * 2) / Board::N_COLS;
	float x = m_WidthMarginFrac + stepX * (float)col + stepX / 2;
	float stepY = (1 - m_HeightMarginFrac * 2) / Board::N_ROWS;
	float y = m_HeightMarginFrac + stepY * (float)row + stepY / 2;
	float r = min(stepX, stepY) / 4;

	SetPieceColor(piece);
	RenderCircle(x, y, r);
}

void Gui::RenderCircle(float x, float y, float r) {
	// how many edges on the circle:
	// the more, the softer the circle but the more expensive the draw

	int steps = 10;
	glBegin(GL_POLYGON);
	for (int i = 0; i < steps; i++) {
		auto angle_theta = (float)((double)i * std::numbers::pi) / ((float)steps / 2.f);
		Vertex(x + r * std::cos(angle_theta), y + r * std::sin(angle_theta));
	}
	glEnd();
}

void Gui::RenderBoard() const {
	for (Row row_i = 0; row_i < Board::N_ROWS; row_i++) {
		for (Col col_i = 0; col_i < Board::N_COLS; col_i++) {
			BoardPiece piece = m_Game.GetBoard().GetPiece(row_i, col_i);
			if (piece != BoardPiece::EMPTY)
				RenderPiece(piece, row_i, col_i);
		}
	}
	ResetColor();
}

void Gui::SetPieceColor(BoardPiece piece) {
	// if the piece is empty, don't bother settings it because the circle
	// should never even be drawn

	if (piece == BoardPiece::P1)
		glColor3f(1.f, 0.f, 0.f);
	else if (piece == BoardPiece::P2)
		glColor3f(0.f, 0.f, 1.f);

}

void Gui::ResetColor() const {
	glColor3fv(m_ResetColor);
}

void Gui::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	LOG(INFO) << "key: " << std::to_string(key)
	<< " scancode: " << std::to_string(scancode)
	<< " action: " << std::to_string(action)
	<< " mods: " << std::to_string(mods);
}