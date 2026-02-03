#include "FoodService.hpp"
#include "models/food.hpp"
#include "utils/Result.hpp"
#include <format>
#include <string>
#include <vector>

namespace cc {
namespace services {

FoodService::FoodService(std::shared_ptr<cc::storage::FoodRepository> repo,
                         std::shared_ptr<cc::clients::OpenFoodFactsClient> off)
    : repo_{repo}, off_{off}

{}

cc::utils::Result<cc::models::Food> FoodService::getOrFetchByBarcode(const std::string& bardcode) {
    cc::utils::Result<cc::models::Food> f;
    f = this->repo_->getById_or_Barcode(bardcode);
    if (f) {
        return f;
    } else {
        f = this->off_->getByBarcode(bardcode);
        if (f) {
            // save food in data base so next time will be available no need to look online
            this->repo_->save(f.unwrap());
            return f;
        }
    }
    return cc::utils::Result<cc::models::Food>::fail(cc::utils::ErrorCode::NotFound,
                                                     "Food not found");
}

// #todo zed der les cas , bach thkam l program
cc::utils::Result<void> FoodService::addManualFood(const cc::models::Food& food) {
    cc::utils::Result<void> result = this->repo_->save(food);
    // if true food is saved correctly
    if (result) {
        return cc::utils::Result<void>::ok();
    } else {
        return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                             "can't add Manual Food");
    }
}

cc::utils::Result<void> FoodService::updateFood(const cc::models::Food& food) {
    cc::utils::Result<void> result = this->repo_->upsert(food);
    // if true food is saved correctly
    if (result) {
        return cc::utils::Result<void>::ok();
    } else {
        return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                             "can't add  or update Food");
    }
}
cc::utils::Result<void> FoodService::deleteFood(const std::string& id) {
    cc::utils::Result<void> result = this->repo_->remove(id);
    if (result) {
        return cc::utils::Result<void>::ok();
    } else {
        return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                             std::format("can't  remove Food with id : {}",id));
    }
}

cc::utils::Result<void> FoodService::clear_data_base() {

    cc::utils::Result<void> result = this->repo_->clear();
    if (result) {
        return cc::utils::Result<void>::ok();
    } else {
        return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                             "can't clear data base");
    }
}

cc::utils::Result<std::vector<cc::models::Food>> FoodService::listFoods(int offset, int limit) {
    cc::utils::Result<std::vector<cc::models::Food>> result = this->repo_->list(offset, limit);
    if (result) {
        return result;
    } else {
        return cc::utils::Result<std::vector<cc::models::Food>>::fail(
            cc::utils::ErrorCode::NotFound, "can't access, or access is forbiden ");
    }
}
} // namespace services
} // namespace cc
