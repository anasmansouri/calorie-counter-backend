#pragma once
#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "storage/FoodRepository.hpp"
#include "storage/MealRepository.hpp"
#include "utils/Result.hpp"
#include <memory>
#include <string>
#include <vector>

namespace cc::storage {
class MealRepository;
}

namespace cc::services {

class MealService {
public:
  MealService(std::shared_ptr<cc::storage::MealRepository> repo);

  cc::utils::Result<std::vector<cc::models::MealLog>>
  getByName(const std::string &name);
  cc::utils::Result<std::vector<cc::models::MealLog>>
  getByDate(int day, int month, int year);
  // cc::utils::Result<std::vector<cc::models::MealLog>> search(const
  // std::string& name,const std::string& date);
  cc::utils::Result<void> addNewMeal(const cc::models::MealLog &meal);
  cc::utils::Result<void> updateMeal(const cc::models::MealLog &meal);
  cc::utils::Result<void> deleteMeal(int id);
  cc::utils::Result<void> clear_data_base();
  cc::utils::Result<std::vector<cc::models::MealLog>> listMeals(int offset = 0,
                                                                int limit = 50);

private:
  std::shared_ptr<cc::storage::MealRepository> repo_;
};

} // namespace cc::services
