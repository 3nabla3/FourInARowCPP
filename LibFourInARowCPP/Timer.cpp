#include "pch.h"
#include "Timer.h"

Timer::Timer(std::string name, std::string prefix)
:m_Name(std::move(name)), m_Prefix(std::move(prefix)){
	Reset();
}

long Timer::MsElapsed() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_beg).count();
}

Timer::~Timer() {
	LOG(INFO) << "[" << m_Prefix << " : " << m_Name << "] "
	<< std::to_string(MsElapsed()) << " ms elapsed";
}

void Timer::Reset() {
	m_beg = Clock::now();
}
