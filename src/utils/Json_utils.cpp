#include "utils/Json_utils.hpp"

namespace cc::utils {
crow::json::wvalue to_crow_json(const nlohmann::json& j) {
    crow::json::wvalue result;

    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            result[it.key()] = to_crow_json(it.value());
        }
    } else if (j.is_array()) {
        crow::json::wvalue::list arr;
        for (auto& el : j) {
            arr.push_back(to_crow_json(el));
        }
        result = std::move(arr);
    } else if (j.is_string()) {
        result = j.get<std::string>();
    } else if (j.is_number_integer()) {
        result = j.get<int>();
    } else if (j.is_number_float()) {
        result = j.get<double>();
    } else if (j.is_boolean()) {
        result = j.get<bool>();
    } else if (j.is_null()) {
        result = nullptr;
    }

    return result;
}
} // namespace cc::utils