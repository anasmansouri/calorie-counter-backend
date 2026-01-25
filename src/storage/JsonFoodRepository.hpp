#pragma once
#include "nlohmann/json.hpp"
#include "storage/FoodRepository.hpp"
#include <mutex>
#include <string>

namespace cc::storage {

class JsonFoodRepository : public FoodRepository {
  public:
    explicit JsonFoodRepository(std::string filePath);

    cc::utils::Result<void> save(const cc::models::Food& food) override;
    cc::utils::Result<cc::models::Food> getById_or_Barcode(const std::string& id) override;
    cc::utils::Result<std::vector<cc::models::Food>> list(int offset = 0, int limit = 50) override;
    cc::utils::Result<void> remove(const std::string& id) override;

    // update or insert if doesn't exist
    cc::utils::Result<void> upsert(const cc::models::Food& food) override;

    // clear all records
    cc::utils::Result<void> clear() override;

    void setFlushOnWrite(bool enable);

    // remove copy and assign because mutex is not copyable
    // but allowing move construtor
    JsonFoodRepository(const JsonFoodRepository&) = delete;
    JsonFoodRepository& operator=(const JsonFoodRepository&) = delete;

    JsonFoodRepository(JsonFoodRepository&&) noexcept = default;
    JsonFoodRepository& operator=(JsonFoodRepository&&) noexcept = default;

  private:
    std::string filePath_;
    bool flushOnWrite_ = true;
    mutable std::mutex mtx_;
};

} // namespace cc::storage
