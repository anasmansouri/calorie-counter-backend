#pragma once
#include "models/daily_log.hpp"
#include "meal_log.hpp"
#include <algorithm>
#include <vector>

namespace cc::models {

DailyLog::DailyLog(std::chrono::system_clock::time_point tsUtc) : tsUtc_{tsUtc} {}
void DailyLog::setDate(std::chrono::system_clock::time_point tsUtc) {
    this->tsUtc_ = tsUtc;
}
void DailyLog::addMeal(const MealLog& meal) {
    this->meals_.push_back(meal);
}

void DailyLog::addMeals(const std::vector<MealLog> meals) {
    this->meals_ = meals;
}

std::vector<MealLog> DailyLog::meals() const {
    return this->meals_;
}
std::chrono::system_clock::time_point DailyLog::getDate() const {
    return this->tsUtc_;
}

} // namespace cc::models
