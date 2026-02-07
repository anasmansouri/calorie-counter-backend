#include "utils/common_functions.hpp"

namespace cc::utils {
bool isBarCodeDigit(std::string_view s) {
  return !s.empty() && std::ranges::all_of(
                           s, [](unsigned char c) { return std::isdigit(c); });
}

std::string expand_user_path(const std::string &path) {
  if (!path.starts_with("~/"))
    return path;

  const char *home = std::getenv("HOME");
  if (!home || std::string(home).empty()) {
    throw std::runtime_error("HOME is not set; cannot expand ~/ path");
  }
  return std::string(home) + path.substr(1); // replace leading '~'
}

std::string  default_meals_db_path(){
  // Prefer XDG_DATA_HOME if available
  const char *xdg = std::getenv("XDG_DATA_HOME");
  if (xdg && std::string(xdg).size() > 0) {
    return std::string(xdg) + "/calorie-counter/json_meals_data_base.json";
  }

  // Fallback to ~/.local/share
  return expand_user_path("~/.local/share/calorie-counter/json_meals_data_base.json");
} 
std::string default_food_db_path() {
  // Prefer XDG_DATA_HOME if available
  const char *xdg = std::getenv("XDG_DATA_HOME");
  if (xdg && std::string(xdg).size() > 0) {
    return std::string(xdg) + "/calorie-counter/json_data_base.json";
  }

  // Fallback to ~/.local/share
  return expand_user_path("~/.local/share/calorie-counter/json_data_base.json");
}

void ensure_db_file_exists(const std::string &db_path) {
  std::filesystem::path p(db_path);

  // Create parent dirs: ~/.local/share/calorie-counter/
  std::filesystem::create_directories(p.parent_path());

  // Create file if missing (init as JSON array)
  if (!std::filesystem::exists(p)) {
    std::ofstream out(p);
    if (!out)
      throw std::runtime_error("Cannot create DB file: " + p.string());
    out << "[]\n";
  }
}




bool canConnectTcp(std::string_view ip, std::uint16_t port) {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) return false;

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  // Convert "127.0.0.1" -> binary IPv4 address
  if (::inet_pton(AF_INET, ip.data(), &addr.sin_addr) != 1) {
    ::close(fd);
    return false;
  }

  bool ok = (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);
  ::close(fd);
  return ok;
}

void waitUntilListening(std::uint16_t port, std::chrono::milliseconds timeout ) {
  using clock = std::chrono::steady_clock;
  auto start = clock::now();

  while (clock::now() - start < timeout) {
    if (canConnectTcp("127.0.0.1", port)) return;
    std::this_thread::sleep_for(std::chrono::milliseconds{20});
  }

  throw std::runtime_error("Server did not start listening in time");
}
} // namespace cc::utils
