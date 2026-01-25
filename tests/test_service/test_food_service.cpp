#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "services/FoodService.hpp"
#include "storage/FoodRepository.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "utils/Result.hpp"
#include "utils/date_time_utils.hpp"
#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <iterator>
#include <magic_enum.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace cc::services;

class FoodServiceTest : public ::testing::Test {
  protected:
    void SetUp() override { // runs BEFORE each TEST_F
                            /*
                    
cc::storage::JsonFoodRepository
                    repo{"/home/anas/personal_projects/calorie-counter-backend/json_data_base.json"};
cc::clients::OpenFoodFactsClient client;
std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr=
                    std::make_shared<cc::storage::JsonFoodRepository>(repo);
std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
                    std::make_shared<cc::clients::OpenFoodFactsClient>(client);                     FoodService
                    food_service{repo_shared_ptr,client_ptr};
*/
    }

    void TearDown() override { // runs AFTER each TEST_F
                               // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
};

TEST_F(FoodServiceTest, listFoods) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
}
TEST_F(FoodServiceTest, listFoods_data_base_path_is_wrong) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anasss/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    ;
    EXPECT_EQ(food_service.clear_data_base().unwrap_error().code,
              cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(food_service.listFoods().unwrap_error().code, cc::utils::ErrorCode::NotFound);
}
TEST_F(FoodServiceTest, addManualFood) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    food_service.addManualFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
}

TEST_F(FoodServiceTest, addManualFood_wrong_path_to_data_base) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anasss/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    food_service.addManualFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap_error().code, cc::utils::ErrorCode::NotFound);
    EXPECT_EQ(food_service.addManualFood(food).unwrap_error().code,
              cc::utils::ErrorCode::StorageError);
}

TEST_F(FoodServiceTest, updateFood) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    food_service.addManualFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
    food.setName("new name");
    food_service.updateFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
    std::remove("/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
}

TEST_F(FoodServiceTest, updateFood_wrong_path_to_data_base) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anasss/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    EXPECT_EQ(food_service.addManualFood(food).unwrap_error().code,
              cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(food_service.updateFood(food).unwrap_error().code,
              cc::utils::ErrorCode::StorageError);
}

TEST_F(FoodServiceTest, deleteFood) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    food_service.addManualFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
    EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
    EXPECT_EQ(food_service.listFoods().unwrap()[0].id(), food.id());
    food_service.deleteFood(food.id());
    std::remove("/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
}

TEST_F(FoodServiceTest, deleteFood_wrong_id) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
    cc::models::Food food;
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setServingSizeG(100);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
    food_service.addManualFood(food);
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
    EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
    EXPECT_EQ(food_service.listFoods().unwrap()[0].id(), food.id());
    std::string wrong_id = std::format("{}+wrong", food.id());
    food_service.deleteFood(wrong_id);
    EXPECT_EQ(food_service.deleteFood(wrong_id).unwrap_error().code,
              cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
    std::remove("/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
}

TEST_F(FoodServiceTest, getOrFetchByBarcode) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    std::string kefir_barcode("4025500287955");
    // Fetch from online data base  because food doesn't exist locally
    cc::utils::Result<cc::models::Food> kefir = food_service.getOrFetchByBarcode(kefir_barcode);
    EXPECT_EQ(kefir.unwrap().id(), kefir_barcode);

    // Fetch from local data base because it is already saved to local data base
    // food_service.addManualFood(kefir.unwrap());
    cc::utils::Result<cc::models::Food> food = food_service.getOrFetchByBarcode(kefir_barcode);
    EXPECT_EQ(food.unwrap().id(), kefir_barcode);
}

TEST_F(FoodServiceTest, getOrFetchByBarcode_wrong_id) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base_temp_.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    FoodService food_service{repo_shared_ptr, client_ptr};
    food_service.clear_data_base();
    std::string wrong_barcode("000000000000000");
    cc::utils::Result<cc::models::Food> food = food_service.getOrFetchByBarcode(wrong_barcode);
    EXPECT_EQ(food.unwrap_error().code, cc::utils::ErrorCode::NotFound);
    // EXPECT_EQ(food.unwrap().id(),kefir_barcode);
}

/*
TEST_F(MealModelTest,setId) {
    // std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    meal.setId("anas");
    EXPECT_EQ(meal.id(),"anas");
    }

TEST_F(MealModelTest,addFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    meal.addFoodItem(minina.id(),50);

    EXPECT_EQ(meal.food_items()[0].first,minina.id());
    EXPECT_EQ(meal.food_items()[0].second,50);
    }

TEST_F(MealModelTest,removeFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    meal.addFoodItem(minina.id(),50);
    EXPECT_EQ(meal.food_items()[0].first,minina.id());
    EXPECT_EQ(meal.food_items()[0].second,50);

    EXPECT_EQ(meal.removeFoodItem(minina.id()),true);
    EXPECT_EQ(meal.food_items().size(),0);
    EXPECT_EQ(meal.removeFoodItem(minina.id()),false);
    }

TEST_F(MealModelTest,setFoodItems) {
    // food item 1
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));

    // food item 1
    cc::models::Food minina_2;
    minina_2.setId("88877788888");
    minina_2.setName("minina_2");
    minina_2.setBrand(std::string("factory_xx"));
    minina_2.setBarcode(std::string("88877788888"));
    minina_2.setCaloriesPer100g(420.0);
    minina_2.setServingSizeG(40.0); // default serving size 40g
    minina_2.setSource(SOURCE::Manual);
    minina_2.setImageUrl(std::string("https://example.com/granola.jpg"));

    std::vector<std::pair<std::string, double>> food_items{{minina.id(),100},{minina_2.id(),50}};

    meal.setFoodItems(food_items);
    EXPECT_EQ(meal.food_items()[0].first,minina.id());
    EXPECT_EQ(meal.food_items()[0].second,100);
    EXPECT_EQ(meal.food_items()[1].first,minina_2.id());
    EXPECT_EQ(meal.food_items()[1].second,50);
}
TEST_F(MealModelTest,from_Json){
    nlohmann::json meal_json;
    meal_json["name"]="Breakfast";
    meal_json["id"]= "100";
    std::vector<std::pair<std::string, double>> food_items{{"002",100},{"009",50}};
    meal_json["foodItems"]=food_items;
    meal_json["tsUtc"]=cc::utils::toIso8601(std::chrono::system_clock::now());
    MealLog meal_lunch = meal_json;
    EXPECT_EQ(meal_json["name"].get<std::string>(),magic_enum::enum_name(meal_lunch.getName()));
    EXPECT_EQ(magic_enum::enum_name(meal_lunch.getName()),meal_json["name"].get<std::string>());
    EXPECT_EQ(meal_json["foodItems"][0][0].get<std::string>(),meal_lunch.food_items()[0].first);
    EXPECT_EQ(meal_json["foodItems"][0][1].get<double>(),meal_lunch.food_items()[0].second);
    EXPECT_EQ(meal_json["tsUtc"].get<std::string>(),cc::utils::toIso8601(meal_lunch.gettime()));
}

TEST_F(MealModelTest,to_Json){
    meal.setName(MEALNAME::Breakfast);
    meal.setId("009900");
    std::vector<std::pair<std::string, double>> food_items{{"002",100},{"009",50}};
    meal.setFoodItems(food_items);
    meal.setTime(std::chrono::system_clock::now());
    ///////////////////////////////////
    nlohmann::json meal_json=meal;
    ///////////////////////////////////
    EXPECT_EQ(meal_json["name"].get<std::string>(),magic_enum::enum_name(meal.getName()));
    EXPECT_EQ(meal_json["id"].get<std::string>(),meal.id());
    EXPECT_EQ(meal_json["foodItems"][0][0].get<std::string>(),meal.food_items()[0].first);
    EXPECT_EQ(meal_json["foodItems"][0][1].get<double>(),meal.food_items()[0].second);
    EXPECT_EQ(meal_json["tsUtc"].get<std::string>(),cc::utils::toIso8601(meal.gettime()));
}
*/
