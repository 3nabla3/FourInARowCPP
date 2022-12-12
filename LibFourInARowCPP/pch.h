#pragma once

#include <cstdbool>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>
#include <cassert>
#include <optional>
#include <array>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <utility>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <limits>
#include <random>
#include <chrono>



#if defined(__linux__)
	#include <bits/stdc++.h>  // apparently, this is needed to iterate over vectors
#elif defined(WIN32)
	#include <Windows.h>
#endif

#include "glog/logging.h"
#include "Timer.h"
