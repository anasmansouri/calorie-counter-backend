#pragma once
#include "models/meal_log.hpp"
#include "utils/Result.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace cc::storage {

class MealRepository {
  public:
    virtual ~MealRepository() = default;


    virtual cc::utils::Result<void> sync_meals_id() = 0;
    virtual cc::utils::Result<void> save(const cc::models::MealLog& meal) = 0;
    virtual cc::utils::Result<cc::models::MealLog> getById(int id) = 0;
    virtual cc::utils::Result<std::vector<cc::models::MealLog>> getByName(cc::models::MEALNAME name) = 0;
    virtual cc::utils::Result<std::vector<cc::models::MealLog>> getByDate(std::chrono::system_clock::time_point tsUtc) = 0;
    virtual cc::utils::Result<std::vector<cc::models::MealLog>> list(int offset = 0,
                                                                     int limit = 50) = 0;
    virtual cc::utils::Result<void> remove(int id) = 0;

    // optional:
    // update or insert if doesn't exist
    virtual cc::utils::Result<void> upsert(const cc::models::MealLog& meal) = 0;
    // clear all records
    virtual cc::utils::Result<void> clear() = 0;
};

} // namespace cc::storage
