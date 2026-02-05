#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "utils/Result.hpp"
#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace cc::clients;

class OpenFoodFactsClientModelTest : public ::testing::Test {
  protected:
    void SetUp() override { // runs BEFORE each TEST_F
    }

    void TearDown() override { // runs AFTER each TEST_F
                               // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
    OpenFoodFactsClient client;
    std::string barcode{"5060245600507"};
    cc::models::Food food;
};

TEST_F(OpenFoodFactsClientModelTest, getByBarcode) {

    OpenFoodFactsClient client_test{};
    std::string barcode_whey_protein{"4250519647425"};
    auto result = client_test.getByBarcode(barcode_whey_protein);

    if (result) {
        food = result.unwrap();
        EXPECT_EQ(food.name(), "Designer Whey Protein Strawberry Cream");
        EXPECT_EQ(food.brand(), "ESN");
        EXPECT_EQ(food.barcode().value(), "4250519647425");
        EXPECT_EQ(food.caloriesPer100g(), 375);
        /////////////////////////////////////////////////////
        EXPECT_NO_THROW(food.imageUrl().value());
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[0].type(), cc::models::NutrientType::Protein);
        EXPECT_EQ(food.nutrients()[0].unit(), "g");
        EXPECT_EQ(food.nutrients()[0].value(), 76);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[1].type(), cc::models::NutrientType::Carbs);
        EXPECT_EQ(food.nutrients()[1].unit(), "g");
        EXPECT_EQ(food.nutrients()[1].value(), 5.7);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[2].type(), cc::models::NutrientType::Fat);
        EXPECT_EQ(food.nutrients()[2].unit(), "g");
        EXPECT_EQ(food.nutrients()[2].value(), 4.6);
    }

    ///////////////////////////////////////////////////////////

    std::string barcode_basmati_reis{"20364625"};
    client_test.updateLanguageCountry("fr");
    result = client_test.getByBarcode(barcode_basmati_reis);

    if (result) {
        food = result.unwrap();
        EXPECT_EQ(food.name(), "Reis - Basmati Reis");
        EXPECT_EQ(food.brand(), "Golden Sun");
        EXPECT_EQ(food.barcode().value(), "20364625");
        EXPECT_EQ(food.caloriesPer100g(), 353);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.imageUrl().value(), "https://images.openfoodfacts.org/images/products/000/"
                                           "002/036/4625/front_fr.73.400.jpg");
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[0].type(), cc::models::NutrientType::Protein);
        EXPECT_EQ(food.nutrients()[0].unit(), "g");
        EXPECT_EQ(food.nutrients()[0].value(), 8.2);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[1].type(), cc::models::NutrientType::Carbs);
        EXPECT_EQ(food.nutrients()[1].unit(), "g");
        EXPECT_EQ(food.nutrients()[1].value(), 78);
        /////////////////////////////////////////////////////
        EXPECT_EQ(food.nutrients()[2].type(), cc::models::NutrientType::Fat);
        EXPECT_EQ(food.nutrients()[2].unit(), "g");
        EXPECT_EQ(food.nutrients()[2].value(), 0.5);
    }
}

TEST_F(OpenFoodFactsClientModelTest, getByBarcode_wrong_barcode) {
    OpenFoodFactsClient client_test{};
    std::string wrong_barcode_whey_protein{std::format("{}{}", "wrong", "5060245600507")};
    auto result = client_test.getByBarcode(wrong_barcode_whey_protein);
    EXPECT_EQ(result.unwrap_error().code, cc::utils::ErrorCode::InvalidInput);
}

TEST_F(OpenFoodFactsClientModelTest, setUserAgent) {
    EXPECT_NO_THROW(
        client.setUserAgent("CalorieCounter/1.0 (+https://anas_mansouri.com; anas@mansouri.com)"));
}

TEST_F(OpenFoodFactsClientModelTest, updateLanguageCountry) {
    EXPECT_NO_THROW(client.updateLanguageCountry("fr"));
}
