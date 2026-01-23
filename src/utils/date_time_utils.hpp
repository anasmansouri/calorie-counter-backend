#pragma once
#include <chrono>
#include <string>
#include <format>
#include <sstream>
#include "nlohmann/json.hpp"

namespace cc::utils
{

    using TimePoint = std::chrono::system_clock::time_point;

    // Convert time_point → ISO 8601 string (UTC)
    inline std::string toIso8601(const TimePoint &tp)
    {
        // Convert to UTC time_t
        auto secs = std::chrono::floor<std::chrono::seconds>(tp);
        return std::format("{:%FT%TZ}", secs); // e.g., "2025-11-08T13:45:00Z"
    }

    // Convert ISO 8601 string → time_point
    inline TimePoint fromIso8601(const std::string &iso)
    {
        std::istringstream iss(iso);
        std::tm tm{};
        iss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        if (iss.fail())
        {
            // handle error: throw, return epoch, or your Result<...>::fail(...)
            return TimePoint{}; // placeholder; integrate with your ErrorCode
        }
        auto t = timegm(&tm); // POSIX: convert UTC struct tm → time_t
        return std::chrono::system_clock::from_time_t(t);
    }
} // namespace cc::utils
