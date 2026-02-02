#pragma once
#include "models/meal_log.hpp"
#include "nlohmann/json.hpp"
#include "storage/MealRepository.hpp"
#include "utils/date_time_utils.hpp"
#include <mutex>
#include <string>
#include <magic_enum.hpp>
namespace cc::storage {

class JsonMealRepository : public MealRepository {
  public:
    explicit JsonMealRepository(std::string filePath);

    // always run it once the repo starts
    cc::utils::Result<void> sync_meals_id() override;
    cc::utils::Result<void> save(const cc::models::MealLog& meal) override;
    cc::utils::Result<cc::models::MealLog> getById(int id) override;
    cc::utils::Result<std::vector<cc::models::MealLog>> getByName(cc::models::MEALNAME name) override;

    cc::utils::Result<std::vector<cc::models::MealLog>> getByDate(std::chrono::system_clock::time_point tsUtc) override;
    cc::utils::Result<std::vector<cc::models::MealLog>> list(int offset = 0,
                                                             int limit = 50) override;
    cc::utils::Result<void> remove(int id) override;

    // update or insert if doesn't exist
    cc::utils::Result<void> upsert(const cc::models::MealLog& meal) override;

    // clear all records
    cc::utils::Result<void> clear() override;

    void setFlushOnWrite(bool enable);

    // remove copy and assign because mutex is not copyable
    // but allowing move construtor
    JsonMealRepository(const JsonMealRepository&) = delete;
    JsonMealRepository& operator=(const JsonMealRepository&) = delete;

    JsonMealRepository(JsonMealRepository&&) noexcept = default;
    JsonMealRepository& operator=(JsonMealRepository&&) noexcept = default;

  private:
    std::string filePath_;
    bool flushOnWrite_ = true;
    mutable std::mutex mtx_;
};

} // namespace cc::storage
