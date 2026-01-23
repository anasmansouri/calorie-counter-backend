#pragma once
#include <vector>
#include "meal_log.hpp"
#include <nlohmann/json.hpp>
#include <chrono>

namespace cc::models
{

  class DailyLog
  {
  public:
    DailyLog(std::chrono::system_clock::time_point tsUtc);

    void setDate(std::chrono::system_clock::time_point tsUtc);
    void addMeal(const MealLog &meal);
    void addMeals(const std::vector<MealLog> meals);
    // getters
    std::vector<MealLog> meals() const;
    std::chrono::system_clock::time_point getDate() const;

  private:
    std::chrono::system_clock::time_point tsUtc_;
    std::vector<MealLog> meals_;
  };

  inline void to_json(nlohmann::json &j, const cc::models::DailyLog &d)
  {
    j = {
        {"meals", d.meals()},
    };
  }

  inline void from_json(const nlohmann::json &j, cc::models::DailyLog &d)
  {
    d.addMeals(j.at("meals").get<std::vector<MealLog>>());
  }

} // namespace cc::models
