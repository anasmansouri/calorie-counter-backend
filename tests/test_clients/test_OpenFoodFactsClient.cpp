#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "models/nutrient.hpp"
#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "utils/Result.hpp"

using namespace cc::clients;

class OpenFoodFactsClientModelTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
    }

    void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
    OpenFoodFactsClient client ;
    std::string barcode{"5060245600507"};
    cc::models::Food food;
};

TEST_F(OpenFoodFactsClientModelTest,getByBarcode) {
    
    OpenFoodFactsClient client_test{};
    std::string barcode_whey_protein{"5060245600507"};
    auto result = client_test.getByBarcode(barcode_whey_protein);
    
    
    if(result){
        food = result.unwrap();
        EXPECT_EQ(food.name(),"Gold Standard 100% Whey Double Rich Chocolate Flavour");
        EXPECT_EQ(food.brand(),"Optimum Nutrition");
        EXPECT_EQ(food.barcode().value(),"5060245600507");
        EXPECT_EQ(food.caloriesPer100g(),374);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.imageUrl().value(),"https://images.openfoodfacts.org/images/products/506/024/560/0507/front_en.27.400.jpg");
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[0].name(),"protein");
        EXPECT_EQ(food.nutrients()[0].unit(),"g");
        EXPECT_EQ(food.nutrients()[0].value(),77.4);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[1].name(),"carbohydrates");
        EXPECT_EQ(food.nutrients()[1].unit(),"g");
        EXPECT_EQ(food.nutrients()[1].value(),5.16);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[2].name(),"fat");
        EXPECT_EQ(food.nutrients()[2].unit(),"g");
        EXPECT_EQ(food.nutrients()[2].value(),4.52);
    }

    ///////////////////////////////////////////////////////////

    std::string barcode_basmati_reis{"20364625"};
    client_test.updateLanguageCountry("fr");
    result = client_test.getByBarcode(barcode_basmati_reis);
    
    if(result){
        food = result.unwrap();
        EXPECT_EQ(food.name(),"Reis - Basmati Reis");
        EXPECT_EQ(food.brand(),"Golden Sun");
        EXPECT_EQ(food.barcode().value(),"20364625");
        EXPECT_EQ(food.caloriesPer100g(),353);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.imageUrl().value(),"https://images.openfoodfacts.org/images/products/000/002/036/4625/front_fr.73.400.jpg");
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[0].name(),"protein");
        EXPECT_EQ(food.nutrients()[0].unit(),"g");
        EXPECT_EQ(food.nutrients()[0].value(),8.2);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[1].name(),"carbohydrates");
        EXPECT_EQ(food.nutrients()[1].unit(),"g");
        EXPECT_EQ(food.nutrients()[1].value(),78);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[2].name(),"fat");
        EXPECT_EQ(food.nutrients()[2].unit(),"g");
        EXPECT_EQ(food.nutrients()[2].value(),0.5);
    }
}

TEST_F(OpenFoodFactsClientModelTest,getByBarcode_wrong_barcode) {
    OpenFoodFactsClient client_test{};
    std::string wrong_barcode_whey_protein{std::format("{}{}","wrong","5060245600507")};
    auto result = client_test.getByBarcode(wrong_barcode_whey_protein);
    EXPECT_EQ(result.unwrap_error().code, cc::utils::ErrorCode::InvalidInput);
    }

TEST_F(OpenFoodFactsClientModelTest,setUserAgent) {
    EXPECT_NO_THROW(client.setUserAgent("CalorieCounter/1.0 (+https://anas_mansouri.com; anas@mansouri.com)"));
    }

TEST_F(OpenFoodFactsClientModelTest,updateLanguageCountry) {
    EXPECT_NO_THROW(client.updateLanguageCountry("fr"));
    }
