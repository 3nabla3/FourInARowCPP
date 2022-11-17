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

#ifdef __linux__
	// #include <bits/stdc++.h>  // apparently, this is needed to iterate over vectors
#else 
	#ifdef WIN32
		#include <Windows.h>
	#endif
#endif

#include "glog/logging.h"