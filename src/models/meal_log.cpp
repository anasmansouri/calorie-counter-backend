#pragma once
#include "models/meal_log.hpp"
#include "models/food.hpp"
#include <algorithm>
#include <bits/chrono.h>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

namespace cc::models {
MealLog::MealLog() : name_{MEALNAME::Lunch},id_{++next_id_} {
    this->tsUtc_ = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
}
MealLog::MealLog(MEALNAME name) : name_{name},id_{++next_id_} {
    this->tsUtc_ = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

}
MEALNAME MealLog::getName() const {
    return this->name_;
}
void MealLog::setName(MEALNAME name) {
    this->name_ = name;
}

int MealLog::id() const {
    return this->id_;
}
std::chrono::system_clock::time_point MealLog::gettime() const {
    return this->tsUtc_;
}

void MealLog::setId(int id) {
    this->id_ = id;
}

void MealLog::setTime(std::chrono::system_clock::time_point tsUtc) {
    this->tsUtc_ = std::chrono::floor<std::chrono::seconds>(tsUtc);
}

void MealLog::setFoodItems(std::vector<std::pair<std::string, double>> food_items) {
    this->food_items_ = food_items;
}

std::vector<std::pair<std::string, double>> MealLog::food_items() const {
    return this->food_items_;
}
// operations
void MealLog::addFoodItem(const std::string& foodId, double grams) {
    this->food_items_.push_back({foodId, grams});
}
bool MealLog::removeFoodItem(const std::string& foodId) {
    for (auto it = this->food_items_.begin(); it != this->food_items_.end(); it++) {
        if (it->first == foodId) {
            this->food_items_.erase(it);
            // maybe you need to remove it from the data base
            return true;
        }
    }
    return false;
}
} // namespace cc::models
