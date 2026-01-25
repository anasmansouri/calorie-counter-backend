#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "utils/Result.hpp"
#include <algorithm>
#include <array>
#include <clients/OpenFoodFactsClient.hpp>
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>
#include <utils/HttpClient.hpp>
#include <utils/common_functions.hpp>
#include <vector>
namespace cc::clients {
OpenFoodFactsClient::OpenFoodFactsClient(const std::string baseUrl, const std::string userAgent)
    : baseUrl_(baseUrl), userAgent_(userAgent) {
    this->locale = "en";
}
cc::utils::Result<cc::models::Food> OpenFoodFactsClient::getByBarcode(const std::string& barcode) {

    if (!(cc::utils::isBarCodeDigit(barcode))) {
        return cc::utils::Result<cc::models::Food>::fail(cc::utils::ErrorCode::InvalidInput,
                                                         "barcode needs to be a number");
    }
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        std::cerr << "curl_global_init failed\n";
    }
    cc::utils::HttpClient http;
    http.setTimeoutMs(5000);
    http.setUserAgents(this->userAgent_);
    // GET https://world.openfoodfacts.org/api/v0/product/{barcode}.json
    // it is mentioned there de "lc=de" for deutchland
    std::string reduce_payload =
        "?lc=" + this->locale +
        "&fields=code,product_name,brands,quantity,serving_size,nutrition_data_per,nutriments,"
        "nutriscore_grade,countries_tags,lc,categories_tags,selected_images,last_modified_t";
    // decltype(http.getJson(std::string{})) r;
    auto r =
        http.getJson(this->baseUrl_ + "/api/v2/product/{" + barcode + "}.json" + reduce_payload);
    if (!r) {
        std::cerr << "GET failed: " << r.unwrap_error().message << "\n";
        return cc::utils::Result<cc::models::Food>::fail(cc::utils::ErrorCode::NetworkError,
                                                         r.unwrap_error().message);
    } else {
        std::cout << "GET succeeded, body size = " << r.unwrap().size() << "\n";
        auto parsed_food = this->parseFoodFromOffJson_barcode(r);
        if (!parsed_food) {
            std::cerr << "parsing failed" << std::endl;
            return cc::utils::Result<cc::models::Food>::fail(cc::utils::ErrorCode::ParseError,
                                                             parsed_food.unwrap_error().message);
        } else {
            std::cout << "parsed succeeded" << std::endl;
            return parsed_food;
        }
    }
    return cc::utils::Result<cc::models::Food>::ok(cc::models::Food());
}

void OpenFoodFactsClient::setUserAgent(const std::string& ua) {
    this->userAgent_ = ua;
}
void OpenFoodFactsClient::updateLanguageCountry(const std::string& locale) {
    this->locale = locale;
    this->baseUrl_ = "https://" + locale + ".openfoodfacts.org";
}

cc::utils::Result<cc::models::Food> OpenFoodFactsClient::parseFoodFromOffJson_barcode(
    cc::utils::Result<std::string> food_in_off_json_format) {
    //  product
    nlohmann::json j = nlohmann::json::parse(food_in_off_json_format.unwrap());
    if (j.contains("product")) {

        auto product = j.at("product");
        cc::models::Food food_item;
        if (product.contains("brands") && !product["brands"].is_null()) {
            food_item.setName(product.at("product_name").get<std::string>());
            food_item.setBrand(product.at("brands").get<std::string>());
        } else {
            food_item.setName("null");
            food_item.setBrand("null");
        }

        if (product.contains("selected_images") && !product["selected_images"].is_null()) {
            if (product.at("selected_images").contains("front") &&
                !product.at("selected_images")["front"].is_null()) {

                if (product.at("selected_images")["front"].contains("display") &&
                    !product.at("selected_images")["front"]["display"].is_null()) {

                    if (product.at("selected_images")["front"]["display"].contains(this->locale) &&
                        !product.at("selected_images")["front"]["display"][this->locale]
                             .is_null()) {
                        food_item.setImageUrl(product.at("selected_images")
                                                  .at("front")
                                                  .at("display")
                                                  .at(this->locale)
                                                  .get<std::string>());
                    } else if (product.at("selected_images")["front"]["display"].contains("de") &&
                               !product.at("selected_images")["front"]["display"]["de"].is_null()) {
                        food_item.setImageUrl(product.at("selected_images")
                                                  .at("front")
                                                  .at("display")
                                                  .at("de")
                                                  .get<std::string>());
                    } else if (product.at("selected_images")["front"]["display"].contains("en") &&
                               !product.at("selected_images")["front"]["display"]["en"].is_null()) {

                        food_item.setImageUrl(product.at("selected_images")
                                                  .at("front")
                                                  .at("display")
                                                  .at("en")
                                                  .get<std::string>());
                    } else if (product.at("selected_images")["front"]["display"].contains("fr") &&
                               !product.at("selected_images")["front"]["display"]["fr"].is_null()) {
                        food_item.setImageUrl(product.at("selected_images")
                                                  .at("front")
                                                  .at("display")
                                                  .at("fr")
                                                  .get<std::string>());
                    }
                }
            }
        } // no need for else already null by default

        // #todo remove this loop maybe
        for (auto it = product.begin(); it != product.end(); ++it) {
            std::cout << " - " << it.key() << std::endl;
        }
        // }
        if (product.contains("code") && !product["code"].is_null()) {
            food_item.setBarcode(product.at("code").get<std::string>());
            food_item.setId(product.at("code").get<std::string>());
        } // no need for else already null by default

        //  nutriments

        if (product.contains("nutriments") && !product["nutriments"].is_null()) {
            auto nutriments = product.at("nutriments");
            std::vector<cc::models::Nutrient> ns;

            // calories
            if (nutriments.contains("energy-kcal_100g") &&
                !nutriments["energy-kcal_100g"].is_null()) {
                food_item.setCaloriesPer100g(nutriments.at("energy-kcal_100g").get<double>());
            }

            // protein

            if (nutriments.contains("proteins") && !nutriments["proteins"].is_null()) {
                models::Nutrient p;
                p.setName("protein");
                p.setUnit(nutriments.at("proteins_unit").get<std::string>());
                p.setValue(nutriments.at("proteins_100g").get<double>());
                ns.push_back(p);
            }

            // carbs
            if (nutriments.contains("carbohydrates") && !nutriments["carbohydrates"].is_null()) {
                models::Nutrient carbs;
                carbs.setName("carbohydrates");
                carbs.setUnit(nutriments.at("carbohydrates_unit"));
                carbs.setValue(nutriments.at("carbohydrates_100g"));
                ns.push_back(carbs);
            }

            //  fat
            if (nutriments.contains("fat") && !nutriments["fat"].is_null()) {
                models::Nutrient fat;
                fat.setName("fat");
                fat.setUnit(nutriments.at("fat_unit"));
                fat.setValue(nutriments.at("fat_100g"));
                ns.push_back(fat);
            }

            food_item.setNutrients(ns);
        }
        return cc::utils::Result<cc::models::Food>::ok(std::move(food_item));
    } else {
        return cc::utils::Result<cc::models::Food>::fail(cc::utils::ErrorCode::NotFound,
                                                         "Product not found");
    }
}
} // namespace cc::clients
