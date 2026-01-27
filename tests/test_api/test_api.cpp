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

#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "api/Server.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>

using namespace cc::api;

class ServerTest : public ::testing::Test {
  protected:
    void SetUp() override { // runs BEFORE each TEST_F
    }

    void TearDown() override { // runs AFTER each TEST_F
                               // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
    Server server; 
};

TEST_F(ServerTest , listFoods) {
    cc::clients::OpenFoodFactsClient client;
    std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base.json");
    std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
        std::make_shared<cc::clients::OpenFoodFactsClient>(client);
    cc::services::FoodService food_service{repo_shared_ptr, client_ptr};

    Server server(8080,std::make_shared<cc::services::FoodService>(food_service)); 
    server.start();
}
/*
TEST_F(NutrientModelTest, setName) {
    nutrient.setName("Carbs");
    EXPECT_EQ(nutrient.name(), "Carbs");
}

TEST_F(NutrientModelTest, setUnit) {
    nutrient.setUnit("g");
    EXPECT_EQ(nutrient.unit(), "g");
}
TEST_F(NutrientModelTest, setValue) {
    nutrient.setValue(45);
    EXPECT_EQ(nutrient.value(), 45);
}
TEST_F(NutrientModelTest, to_json) {
    Nutrient nutrient("Protein", 35, "g");
    nlohmann::json nutrient_json_format = nutrient;
    EXPECT_EQ(nutrient_json_format["name"].get<std::string>(), nutrient.name());
    EXPECT_EQ(nutrient_json_format["value"].get<double>(), nutrient.value());
    EXPECT_EQ(nutrient_json_format["unit"].get<std::string>(), nutrient.unit());
}
TEST_F(NutrientModelTest, from_json) {
    nlohmann::json nutrient_json_format = {{"name", "Protein"}, {"value", 100}, {"unit", "g"}};
    Nutrient nutrient = nutrient_json_format;
    EXPECT_EQ(nutrient.name(), nutrient_json_format["name"].get<std::string>());
    EXPECT_EQ(nutrient.value(), nutrient_json_format["value"].get<double>());
    EXPECT_EQ(nutrient.unit(), nutrient_json_format["unit"].get<std::string>());
}
TEST_F(NutrientModelTest, to_string) {
    Nutrient nutrient("Protein", 35, "g");
    std::string nutrient_string =
        std::format("{} : {} : {}", nutrient.name(), nutrient.value(), nutrient.unit());
    EXPECT_EQ(nutrient_string, nutrient.to_string());
}
*/
