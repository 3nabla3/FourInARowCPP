#pragma once
#include <chrono>

class Timer {
public:
	explicit Timer(std::string name, std::string prefix = "");
	void Reset();
	[[nodiscard]] long MsElapsed() const;
	~Timer();

private:
	std::string m_Name;
	std::string m_Prefix;

	// Type alias to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	std::chrono::time_point<Clock> m_beg { Clock::now() };
};

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)

#ifdef NDEBUG
#define SCOPED_TIMER(name)
#else
#define SCOPED_TIMER(name) Timer MAKE_UNIQUE(timer)(name, __func__)
#endif

