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

#include <cstdint>
#include <thread>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace cc::utils {
bool isBarCodeDigit(std::string_view s);
std::string expand_user_path(const std::string &path);
std::string default_food_db_path();
std::string default_meals_db_path();
void ensure_db_file_exists(const std::string &db_path);
bool canConnectTcp(std::string_view ip, std::uint16_t port);
void waitUntilListening(std::uint16_t port, std::chrono::milliseconds timeout);
} // namespace cc::utils
