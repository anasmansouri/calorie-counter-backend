#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>
#include "models/daily_log.hpp"
#include "models/meal_log.hpp"
#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "nlohmann/json.hpp"
#include <magic_enum.hpp>
using namespace cc::models;

class FoodModelTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
            food.setId("00000");                                      
            food.setName("minina");                                      
            food.setBrand("Aicha");                                      
            food.setBarcode("0707070");                                      
            food.setCaloriesPer100g(200);                                      
            food.setServingSizeG(100);
            food.setSource(SOURCE::Manual);
            food.setImageUrl(std::string("https://example.com/granola.jpg"));
    }

    void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
    Food food;
};

TEST_F(FoodModelTest,initializition) {
    std::vector<Nutrient> nutrients {Nutrient{"Protein",24,"g"},Nutrient{"Carbs",100,"g"}};
    Food food{"0000","minina",200,nutrients,200,"0707070","Aicha",std::string("https://example.com/granola.jpg")};
    EXPECT_EQ(food.id(),"0000");
    EXPECT_EQ(food.name(),"minina");
    EXPECT_EQ(food.brand(),"Aicha");
    EXPECT_EQ(food.barcode(),"0707070");
    EXPECT_EQ(food.caloriesPer100g(),200);
    EXPECT_EQ(food.servingSizeG(),200);
    EXPECT_EQ(food.nutrients()[0].name(),"Protein");
    EXPECT_EQ(food.nutrients()[1].name(),"Carbs");
    }
TEST_F(FoodModelTest,setId) {
    Food food;
    food.setId("0000");
    EXPECT_EQ(food.id(),"0000");
    }
TEST_F(FoodModelTest,setName) {
    Food food;
    food.setName("minina");
    EXPECT_EQ(food.name(),"minina");
    }
TEST_F(FoodModelTest,setBrand) {
    Food food;
    food.setBrand("Aicha");
    EXPECT_EQ(food.brand(),"Aicha");
    }
TEST_F(FoodModelTest,setBarcode) {
    Food food;
    food.setBarcode("1946753214860");
    EXPECT_EQ(food.barcode(),"1946753214860");
    }
TEST_F(FoodModelTest,setCaloriesPer100g) {
    Food food;
    food.setCaloriesPer100g(500);
    EXPECT_EQ(food.caloriesPer100g(),500);
    }
TEST_F(FoodModelTest,setServingSizeG) {
    Food food;
    food.setServingSizeG(100);
    EXPECT_EQ(food.servingSizeG(),100);
    }
TEST_F(FoodModelTest,setNutrients) {
    Food food;
    std::vector<Nutrient> nutrients {Nutrient{"Protein",24,"g"},Nutrient{"Carbs",100,"g"}};
    food.setNutrients(nutrients);
    EXPECT_EQ(food.nutrients()[0].name(),"Protein");
    EXPECT_EQ(food.nutrients()[1].name(),"Carbs");
    }
TEST_F(FoodModelTest,setSource){
    Food food;
    food.setSource(SOURCE::Manual);
    EXPECT_EQ(food.source(),SOURCE::Manual);
    }
TEST_F(FoodModelTest,to_Json){
    std::vector<Nutrient> nutrients {Nutrient{"Protein",24,"g"},Nutrient{"Carbs",100,"g"}};
    food.setNutrients(nutrients);
    nlohmann::json food_json =food;
    EXPECT_EQ(food_json["name"].get<std::string>(),"minina");
    EXPECT_EQ(food_json["id"].get<std::string>(),"00000");
    EXPECT_EQ(food_json["caloriesPer100g"].get<double>(),200);
    EXPECT_EQ(food_json["servingSizeG"].get<double>(),100);
    EXPECT_EQ(food_json["nutrient"][0]["name"].get<std::string>(),"Protein");
    EXPECT_EQ(food_json["nutrient"][1]["name"].get<std::string>(),"Carbs");
    }

TEST_F(FoodModelTest,setImageUrl){
    Food food;
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    EXPECT_EQ(food.imageUrl(),std::string("https://example.com/granola.jpg"));
    }

TEST_F(FoodModelTest,from_Json){
    nlohmann::json food_json;
    food_json["name"]="maticha";
    food_json["id"]="0099";
    food_json["caloriesPer100g"]=90;
    food_json["servingSizeG"]=100;
    food_json["barcode"]="0000111999";
    food_json["brand"]="Aicha";
    food_json["imageUrl"]="https://example.com/granola.jpg";
    food_json["source"]=magic_enum::enum_name(SOURCE::Online);
// #####################
    nlohmann::json nutrient_json;
    nutrient_json["name"]="Protein";
    nutrient_json["value"]=10;
    nutrient_json["unit"]="g";
    food_json["nutrient"]=nlohmann::json::array();
    food_json["nutrient"].push_back(nutrient_json);
// #####################
    Food food=food_json;
    
    EXPECT_EQ(food.name(),food_json["name"].get<std::string>());
    EXPECT_EQ(food.id(),food_json["id"].get<std::string>());
    EXPECT_EQ(food.caloriesPer100g(),food_json["caloriesPer100g"].get<double>());
    EXPECT_EQ(food.servingSizeG(),food_json["servingSizeG"].get<double>());
    EXPECT_EQ(food.barcode(),food_json["barcode"].get<std::string>());
    EXPECT_EQ(food.brand(),food_json["brand"].get<std::string>());
    EXPECT_EQ(food.imageUrl(),food_json["imageUrl"].get<std::string>());
    EXPECT_EQ(magic_enum::enum_name((food.source())),food_json["source"].get<std::string>());
    EXPECT_EQ(food.nutrients()[0].name(),food_json["nutrient"][0]["name"].get<std::string>());
} 

TEST_F(FoodModelTest,totalKcal){
    EXPECT_EQ(food.totalKcal(),food.servingSizeG().value()*food.caloriesPer100g());
}

TEST_F(FoodModelTest,to_string){
    std::string food_string= std::format("{} : {} : {}",food.name(),food.id(),food.servingSizeG().value());
    EXPECT_EQ(food_string,food.to_string());
}

class SOURCEModelTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
    }

    void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
};
