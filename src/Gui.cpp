#include "Gui.h"
#include "glog/logging.h"
#include "Timer.h"
#include <thread>

Gui::Gui(const Game& game, uint16_t width, uint16_t height)
		: m_Game(game), m_Width(width), m_Height(height) {
	LOG(INFO) << "Attempting to initialize window";
	if (!glfwInit())
		LOG(FATAL) << "Could not initialize glfw";

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", nullptr, nullptr);
	if (!m_Window) {
		glfwTerminate();
		LOG(FATAL) << "Could not create window";
	}

	glfwMakeContextCurrent(m_Window);
	LOG(INFO) << "Completed windows initialization";
}

void Gui::Run() {
	using namespace std::literals::chrono_literals;

	LOG(INFO) << "Starting main loop";
	while (!glfwWindowShouldClose(m_Window)) {
		static int b = __COUNTER__;
		SCOPED_TIMER("Frame time");
		{
			// SCOPED_TIMER("Another frame time");
		}
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		RenderGrid();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
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
	float startX = m_WidthMarginPercent;
	float endX = 1 - startX;
	float stepX = (endX - startX) / Board::N_COLS;

	// for some reason, clang-tidy doesn't like for loop with floats
	float x = startX;
	while (x < endX) {
		Vertex(x, m_HeightMarginPercent);
		Vertex(x, 1 - m_HeightMarginPercent);
		x += stepX;
	}
}

void Gui::RenderHorizontalLines() const {
	float startY = m_HeightMarginPercent;
	float endY = 1 - startY;
	float stepY = (endY - startY) / Board::N_ROWS;

	float y = startY;
	while (y < endY) {
		Vertex(m_WidthMarginPercent, y);
		Vertex(1 - m_WidthMarginPercent, y);
		y += stepY;
	}
}