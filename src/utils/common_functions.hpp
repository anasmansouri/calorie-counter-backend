#pragma once
#include <algorithm>
#include <cctype>
#include <chrono>
#include <format>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>

#include "nlohmann/json.hpp"

namespace cc::utils {
bool isBarCodeDigit(std::string_view s);
} // namespace cc::utils
