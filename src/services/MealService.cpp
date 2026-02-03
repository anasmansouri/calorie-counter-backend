#include "MealService.hpp"

#include <chrono>
#include <magic_enum.hpp>
#include <optional>
#include <vector>

#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "storage/MealRepository.hpp"
#include "utils/Result.hpp"

namespace cc {
namespace services {

MealService::MealService(std::shared_ptr<cc::storage::MealRepository> repo)
    : repo_{repo}

{}

// #todo zed der les cas , bach thkam l program
cc::utils::Result<void> MealService::addNewMeal(
    const cc::models::MealLog& meal) {
  cc::utils::Result<void> result = this->repo_->save(meal);
  // if true food is saved correctly
  if (result) {
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't add new meal");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>> MealService::getByName(
    const std::string& name) {
  std::optional<cc::models::MEALNAME> optional_name =
      magic_enum::enum_cast<cc::models::MEALNAME>(name);
  cc::models::MEALNAME searched_meal_name;
  if (optional_name.has_value()) {
    searched_meal_name = optional_name.value();
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound, "can't access, or access is forbiden ");
  }
  cc::utils::Result<std::vector<cc::models::MealLog>> meal_list =
      this->repo_->getByName(searched_meal_name);
  if (meal_list) {
    return meal_list;
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound, "can't access, or access is forbiden ");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>> MealService::getByDate(
    int day, int month, int year) {
  std::chrono::year y{year};
  std::chrono::month m{static_cast<unsigned>(month)};
  std::chrono::day d{static_cast<unsigned>(day)};
  const std::chrono::year_month_day ymd{y, m, d};
  if (!ymd.ok()) {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound, "wrong date format");
  }
  std::chrono::sys_days sd{ymd};
  std::chrono::system_clock::time_point tp{sd.time_since_epoch()};

  cc::utils::Result<std::vector<cc::models::MealLog>> meal_list =
      this->repo_->getByDate(tp);
  if (meal_list) {
    return meal_list;
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound, "can't access, or access is forbiden ");
  }
}

cc::utils::Result<void> MealService::updateMeal(
    const cc::models::MealLog& meal) {
  cc::utils::Result<void> result = this->repo_->upsert(meal);
  // if true food is saved correctly
  if (result) {
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't add or update new meal");
  }
}

cc::utils::Result<void> MealService::deleteMeal(int id) {
  cc::utils::Result<void> result = this->repo_->remove(id);
  if (result) {
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't  remove Food with id " + id);
  }
}

cc::utils::Result<void> MealService::clear_data_base() {
  cc::utils::Result<void> result = this->repo_->clear();
  if (result) {
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't clear data base");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>> MealService::listMeals(
    int offset, int limit) {
  cc::utils::Result<std::vector<cc::models::MealLog>> result =
      this->repo_->list(offset, limit);
  if (result) {
    return result;
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound, "can't access, or access is forbiden ");
  }
}

}  // namespace services
}  // namespace cc
