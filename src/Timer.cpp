#include "Timer.h"

#include <utility>

Timer::Timer(std::string name, std::string prefix)
:m_Name(std::move(name)), m_Prefix(std::move(prefix)){
	Reset();
}

long Timer::MsElapsed() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_beg).count();
}

Timer::~Timer() {
	DLOG_EVERY_N(INFO, 10) << "[" << m_Prefix << "] " << std::to_string(MsElapsed()) << " ms elapsed";
}

void Timer::Reset() {
	m_beg = Clock::now();
}
