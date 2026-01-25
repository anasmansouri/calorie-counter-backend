#pragma once
#include "utils/date_time_utils.hpp"
#include <chrono>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>
namespace cc::models {

enum class MEALNAME { Breakfast, Lunch, Dinner, Snack }; // enum::MEALNAME

class MealLog {
  private:
    std::chrono::system_clock::time_point tsUtc_;
    std::string id_;
    MEALNAME name_{MEALNAME::Lunch};
    std::vector<std::pair<std::string, double>> food_items_; // foodId, grams

  public:
    // constructors
    MealLog() = default;
    MealLog(MEALNAME mealName);

    // setters
    void setName(MEALNAME name);
    void setId(std::string id);
    void setTime(std::chrono::system_clock::time_point tsUtc);
    void setFoodItems(std::vector<std::pair<std::string, double>> food_items);

    // getters
    MEALNAME getName() const;
    std::string id() const;
    std::chrono::system_clock::time_point gettime() const;
    std::vector<std::pair<std::string, double>> food_items() const;
    // operations
    void addFoodItem(const std::string& foodId, double grams);
    bool removeFoodItem(const std::string& foodId);
}; // MealLog
inline void to_json(nlohmann::json& j, const cc::models::MealLog& m) {
    j = {{"name", magic_enum::enum_name(m.getName())},
         {"id", m.id()},
         {
             "foodItems",
             m.food_items(),
         },
         {"tsUtc", cc::utils::toIso8601(m.gettime())}};
}
// mrowena had l function
inline void from_json(const nlohmann::json& j, cc::models::MealLog& m) {
    auto meal_name = magic_enum::enum_cast<MEALNAME>(j.at("name").get<std::string>());
    if (meal_name) {
        m.setName(meal_name.value());
    } else {
        m.setName(MEALNAME::Breakfast);
    }
    m.setId(j.at("id").get<std::string>());
    m.setTime(cc::utils::fromIso8601(j.at("tsUtc").get<std::string>()));
    m.setFoodItems(j.at("foodItems").get<std::vector<std::pair<std::string, double>>>());
}
} // namespace cc::models
