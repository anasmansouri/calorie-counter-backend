#pragma once
#include "nlohmann/json.hpp"
#include "storage/MealRepository.hpp"
#include "utils/date_time_utils.hpp"
#include <mutex>
#include <string>
namespace cc::storage {

class JsonMealRepository : public MealRepository {
  public:
    explicit JsonMealRepository(std::string filePath);

    cc::utils::Result<void> save(const cc::models::MealLog& meal) override;
    cc::utils::Result<cc::models::MealLog> getById(const std::string& id) override;
    cc::utils::Result<std::vector<cc::models::MealLog>> list(int offset = 0,
                                                             int limit = 50) override;
    cc::utils::Result<void> remove(const std::string& id) override;

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