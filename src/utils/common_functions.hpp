#pragma once
#include <algorithm>
#include <cctype>
#include <chrono>
#include <string>
#include <format>
#include <sstream>
#include <ranges>
#include <string_view>

#include "nlohmann/json.hpp"

namespace cc::utils
{
    bool isBarCodeDigit(std::string_view s);
} // namespace cc::utils
