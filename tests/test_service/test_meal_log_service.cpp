#include "models/meal_log.hpp"
#include "services/MealService.hpp"
#include "storage/JsonMealRepository.hpp"
#include "utils/Result.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <magic_enum.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace cc::services;

class MealServiceTest : public ::testing::Test {
protected:
  void SetUp() override { // runs BEFORE each TEST_F
  }

  void TearDown() override { // runs AFTER each TEST_F
                             // nothing to destroy //
  }
  // helper functions and members visible to all TEST_F in this suite
  std::string path_to_meal_temp_db{"/tmp/cc_UT_test_service_meal_db.json"};
  std::string wrong_path_to_meal_temp_db{"/tmmp/cc_UT_test_meal_db.json"};
};

TEST_F(MealServiceTest, addNewMeal) {
  std::shared_ptr<cc::storage::JsonMealRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonMealRepository>(path_to_meal_temp_db);
  MealService meal_service{repo_shared_ptr};
  meal_service.clear_data_base();
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 0);
  cc::models::MealLog lunch;
  EXPECT_NO_THROW(meal_service.addNewMeal(lunch));
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 1);
}

TEST_F(MealServiceTest, getByName) {
  std::shared_ptr<cc::storage::JsonMealRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonMealRepository>(path_to_meal_temp_db);
  MealService meal_service{repo_shared_ptr};
  meal_service.clear_data_base();
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 0);
  cc::models::MealLog lunch;
  lunch.setName(cc::models::MEALNAME::Lunch);

  cc::models::MealLog breakfast;
  breakfast.setName(cc::models::MEALNAME::Breakfast);
  EXPECT_NO_THROW(meal_service.addNewMeal(lunch));
  EXPECT_NO_THROW(meal_service.addNewMeal(breakfast));
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 2);
  auto result= meal_service.getByName("Lunch");
  EXPECT_EQ(result.unwrap()[0].getName(),lunch.getName());
}

TEST_F(MealServiceTest, getByDate_same_day_returns_all_meals_in_that_day) {
  using namespace std::chrono;

  auto repo = std::make_shared<cc::storage::JsonMealRepository>(path_to_meal_temp_db);
  cc::services::MealService meal_service{repo};

  // Arrange
  EXPECT_TRUE(meal_service.clear_data_base());
  auto empty = meal_service.listMeals();
  ASSERT_TRUE(empty);
  EXPECT_EQ(empty.unwrap().size(), 0);

  cc::models::MealLog::next_id_.store(0);

  // Build "2026-02-02 00:00:00 UTC" using sys_days
  const year_month_day ymd{year{2026}, month{2}, day{2}};
  ASSERT_TRUE(ymd.ok());

  const sys_days day_start_days{ymd};
  const system_clock::time_point day_start{day_start_days.time_since_epoch()};

  // Two meals on SAME day (2026-02-02), different times
  cc::models::MealLog breakfast;
  breakfast.setName(cc::models::MEALNAME::Breakfast);
  breakfast.setTime(day_start + hours{8} + minutes{15});   // 2026-02-02 08:15 UTC

  cc::models::MealLog lunch;
  lunch.setName(cc::models::MEALNAME::Lunch);
  lunch.setTime(day_start + hours{13} + minutes{5});       // 2026-02-02 13:05 UTC

  // One meal on NEXT day
  cc::models::MealLog dinner;
  dinner.setName(cc::models::MEALNAME::Dinner);
  dinner.setTime(day_start + days{1} + hours{19});         // 2026-02-03 19:00 UTC

  ASSERT_TRUE(meal_service.addNewMeal(breakfast));
  ASSERT_TRUE(meal_service.addNewMeal(lunch));
  ASSERT_TRUE(meal_service.addNewMeal(dinner));

  auto all = meal_service.listMeals();
  ASSERT_TRUE(all);
  EXPECT_EQ(all.unwrap().size(), 3);

  // query meals by date 02/02/2026
  auto res = meal_service.getByDate(2, 2, 2026);

  // Assert
  ASSERT_TRUE(res);
  const auto& meals = res.unwrap();
  EXPECT_EQ(meals.size(), 2);

  // check that the returned meals contain Breakfast and Lunch
  bool hasBreakfast = false;
  bool hasLunch = false;
  for (const auto& m : meals) {
    if (m.getName() == cc::models::MEALNAME::Breakfast) hasBreakfast = true;
    if (m.getName() == cc::models::MEALNAME::Lunch) hasLunch = true;
  }
  EXPECT_TRUE(hasBreakfast);
  EXPECT_TRUE(hasLunch);

  //next day should return only Dinner
  auto res2 = meal_service.getByDate(3, 2, 2026);
  ASSERT_TRUE(res2);
  EXPECT_EQ(res2.unwrap().size(), 1);
  EXPECT_EQ(res2.unwrap()[0].getName(), cc::models::MEALNAME::Dinner);
}

TEST_F(MealServiceTest, UpdateMeal) {
  std::shared_ptr<cc::storage::JsonMealRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonMealRepository>(path_to_meal_temp_db);
  MealService meal_service{repo_shared_ptr};
  meal_service.clear_data_base();
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 0);
  cc::models::MealLog lunch;
  EXPECT_NO_THROW(meal_service.addNewMeal(lunch));
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 1);
  lunch.setFoodItems({{"22128867",25}});
  lunch.setName(cc::models::MEALNAME::Snack);
  meal_service.updateMeal(lunch);
  EXPECT_EQ(meal_service.listMeals().unwrap()[0].getName(),cc::models::MEALNAME::Snack);
}

TEST_F(MealServiceTest, deleteMeal) {
  std::shared_ptr<cc::storage::JsonMealRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonMealRepository>(path_to_meal_temp_db);
  MealService meal_service{repo_shared_ptr};
  meal_service.clear_data_base();
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 0);
  cc::models::MealLog lunch;
  EXPECT_NO_THROW(meal_service.addNewMeal(lunch));
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 1);
  meal_service.deleteMeal(lunch.id());
  EXPECT_EQ(meal_service.listMeals().unwrap().size(), 0);
  std::remove(path_to_meal_temp_db.c_str());
}
