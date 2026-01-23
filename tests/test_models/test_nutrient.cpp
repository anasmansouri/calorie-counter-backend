#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>
#include "models/nutrient.hpp"
#include "models/food.hpp"

using namespace cc::models;

class NutrientModelTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
    }

    void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
     Nutrient nutrient;
};

TEST_F(NutrientModelTest,initializition) {
    Nutrient nutrient("Protein",35,"g");
    EXPECT_EQ(nutrient.name(),"Protein");
    EXPECT_EQ(nutrient.unit(),"g");
    EXPECT_EQ(nutrient.value(),35);
    }

TEST_F(NutrientModelTest,setName) {
    nutrient.setName("Carbs");
    EXPECT_EQ(nutrient.name(),"Carbs");
    }

TEST_F(NutrientModelTest,setUnit) {
    nutrient.setUnit("g");
    EXPECT_EQ(nutrient.unit(),"g");
    }
TEST_F(NutrientModelTest,setValue) {
    nutrient.setValue(45);
    EXPECT_EQ(nutrient.value(),45);
    }
TEST_F(NutrientModelTest,to_json) {
    Nutrient nutrient("Protein",35,"g");
    nlohmann::json nutrient_json_format = nutrient;
    EXPECT_EQ(nutrient_json_format["name"].get<std::string>(),nutrient.name());
    EXPECT_EQ(nutrient_json_format["value"].get<double>(),nutrient.value());
    EXPECT_EQ(nutrient_json_format["unit"].get<std::string>(),nutrient.unit());
    }
TEST_F(NutrientModelTest,from_json) {
    nlohmann::json nutrient_json_format = {{"name","Protein"},{"value",100},{"unit","g"}};
    Nutrient nutrient = nutrient_json_format;
    EXPECT_EQ(nutrient.name(),nutrient_json_format["name"].get<std::string>());
    EXPECT_EQ(nutrient.value(),nutrient_json_format["value"].get<double>());
    EXPECT_EQ(nutrient.unit(),nutrient_json_format["unit"].get<std::string>());
    }
TEST_F(NutrientModelTest,to_string){
    Nutrient nutrient("Protein",35,"g");
    std::string nutrient_string= std::format("{} : {} : {}",nutrient.name(),nutrient.value(),nutrient.unit());
    EXPECT_EQ(nutrient_string,nutrient.to_string());
}
