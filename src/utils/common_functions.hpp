#pragma once
#include "nlohmann/json.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace cc::utils {
bool isBarCodeDigit(std::string_view s);
std::string expand_user_path(const std::string &path);
std::string default_db_path();
void ensure_db_file_exists(const std::string &db_path);
} // namespace cc::utils
