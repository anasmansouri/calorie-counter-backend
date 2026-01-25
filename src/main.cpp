#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "models/nutrient.hpp"
#include "nlohmann/json.hpp"
#include "services/FoodService.hpp"
#include "storage/FoodRepository.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "utils/HttpClient.hpp"
#include "utils/Result.hpp"
#include <api/Server.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
int main() {
    // TODO: load config, init services, run server
    // basmati rice
    // cc::models::Food f = cc::models::Food();
    // f.setName("basmati Rice");
    // f.setId("reference");
    // f.setBrand("lala fatima");
    // f.setBarcode("xxxxxxxx");
    // f.setCaloriesPer100g(200);
    // std::cout<<"food :"<<f.name()<<std::endl;
    // std::cout<<"food :"<<f.name()<<std::endl;
    // #################################################
    // 1) Build some nutrients
    // cc::models::Nutrient protein{"protein", 10.2, "g"}; // per 100g
    // cc::models::Nutrient fat{"fat", 5.8, "g"};
    // cc::models::Nutrient carbs{"carbohydrates", 42.0, "g"};

    // 2) Build a Food
    // cc::models::Food f;
    // f.setId("test-uuid-1234");
    // f.setName("Granola Bar");
    // f.setBrand(std::string("FooBrand"));
    // f.setBarcode(std::string("7622210449283"));
    // f.setCaloriesPer100g(420.0);
    // f.setServingSizeG(40.0); // default serving size 40g
    // f.setSource("manual");
    // f.setImageUrl(std::string("https://example.com/granola.jpg"));

    // add nutrients (mutable access)
    // std::vector<cc::models::Nutrient> temp_nutrients{protein, fat, carbs};
    // f.setNutrients(temp_nutrients);
    // std::vector<std::pair<std::string,double>>v; // items of {foodId, grams}
    // std::pair<std::string,double>pair_anas={f.id(),f.servingSizeG().value()};
    // v.push_back(pair_anas);
    // cc::models::MealLog lunch(cc::models::MEALNAME::Lunch,v);
    // nlohmann::json nutrient_json = protein;
    // nlohmann::json food_json = f;
    // nlohmann::json lunch_json = lunch;
    // std::cout<<food_json.dump(4)<<std::endl;
    // std::cout<<nutrient_json.dump(4)<<std::endl;
    // std::cout<<lunch_json.dump(4)<<std::endl;
    // cc::storage::JsonFoodRepository json_repo("~/serialize_food");
    // json_repo.save(f);
    // ######################

    /*
    // you need to initialise always before start the use of httpclient
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        std::cerr << "curl_global_init failed\n";
        return 1;
    }

    cc::utils::HttpClient http;
    http.setTimeoutMs(5000);

    // 1) Basic GET
    auto r_temp = http.get("https://httpbin.org/get");
    if (!r_temp) {
        std::cerr << "GET failed: " << r_temp.unwrap_error().message << "\n";
    } else {
        std::cout << "GET succeeded, body size = " << r_temp.unwrap().size() << "\n";
        std::cout << "Body: " << r_temp.unwrap().substr(0, 200) << "...\n";
    }

    // 2) Redirect test
    auto r2_temp = http.get("http://httpbin.org/redirect/1");
    if (!r2_temp) {
        std::cerr << "Redirect failed: " << r2_temp.unwrap_error().message << "\n";
    } else {
        std::cout << "Redirect ok, body size = " << r2_temp.unwrap().size() << "\n";
    }

    // 3) Error test (404)
    auto r3_temp = http.get("https://httpbin.org/status/404");
    if (!r3_temp) {
        std::cout << "Correctly got error on 404: " << r3_temp.unwrap_error().message << "\n";
    } else {
        std::cerr << "Unexpected success for 404!\n";
    }
    // test json
    auto temp = http.getJson("https://httpbin.org/json");
    if(!temp){
        std::cerr<<"getJson failed: "<<temp.unwrap_error().message<<"\n";
    }else{
        std::cout<<"JSON BODY"<<temp.unwrap().substr(0,200)<<"\n";
    }

    curl_global_cleanup();
    */
    // at the end always cleanup with this function .
    // #######################################################

    // not implemented yet
    // std::string searched_item{"milk"};
    // std::cout<<"search for "<<searched_item<<std::endl;
    // cc::clients::OpenFoodFactsClient openfoodfactsclient{};
    // auto result = openfoodfactsclient.searchByName(searched_item.c_str());
    // if(result){
    // auto items = result.unwrap();
    // std::cout<<"numbers for 100g"<<std::endl;
    // for(auto i : items){
    // std::cout<<i.name()<<std::endl;
    // if(i.nutrients().empty()){
    // std::cout<<"nutrients is empty"<<std::endl;
    // }

    // for(auto n:i.nutrients()){
    // std::cout<<n.name()<<" : "<<n.value()<<" "<<n.unit()<<std::endl;
    // }
    // }
    // }

    /// ##############################
    ///

    // Barilla
    std::string barcode{"8076809543934"};
    // 6111242100985
    // std::string barcode{"8076809543934"};
    std::cout << "#########################" << std::endl;
    std::cout << "TEST barcode" << std::endl;
    std::cout << "#########################" << std::endl;
    cc::clients::OpenFoodFactsClient openfoodfactsclient{};
    auto result_barcode = openfoodfactsclient.getByBarcode(barcode);

    cc::models::Food food_barcode;
    if (result_barcode) {
        food_barcode = result_barcode.unwrap();
        if (food_barcode.nutrients().empty()) {
            std::cout << "nutrients is empty" << std::endl;
        }
        std::cout << food_barcode.name() << std::endl;
        for (auto n : food_barcode.nutrients()) {
            std::cout << n.name() << " : " << n.value() << " " << n.unit() << std::endl;
        }
    }

    std::string barcode_2{"5449000227041"};
    auto result_barcode_2 = openfoodfactsclient.getByBarcode(barcode_2);
    cc::models::Food food_barcode_2;
    if (result_barcode_2) {
        food_barcode_2 = result_barcode_2.unwrap();
        if (food_barcode_2.nutrients().empty()) {
            std::cout << "nutrients is empty" << std::endl;
        }
        std::cout << food_barcode_2.name() << std::endl;
        for (auto n : food_barcode_2.nutrients()) {
            std::cout << n.name() << " : " << n.value() << " " << n.unit() << std::endl;
        }
    }
    // // ############ json save ####################################

    cc::storage::JsonFoodRepository jsonFoodRepository(
        "/home/anas/personal_projects/calorie-counter-backend/json_data_base.json");
    jsonFoodRepository.save(food_barcode);
    jsonFoodRepository.save(food_barcode_2);

    // ############ get by id from data base(json for now ) ##########
    std::cout << "testing get by id from data base (json for now )" << std::endl;
    cc::utils::Result<cc::models::Food> founded_food =
        jsonFoodRepository.getById_or_Barcode("8076809543934");
    if (founded_food) {
        std::cout << founded_food.unwrap().to_string() << std::endl;
    } else {
        std::cout << founded_food.unwrap_error().message << std::endl;
    }

    // ############ get by id from data base(json for now ) ##########
    std::cout << "testing get by barcode from data base (json for now )" << std::endl;
    founded_food = jsonFoodRepository.getById_or_Barcode("5449000227041");
    if (founded_food) {
        std::cout << founded_food.unwrap().to_string() << std::endl;
    } else {
        std::cout << founded_food.unwrap_error().message << std::endl;
    }
    // ############ list ##########
    std::cout << "testing listing food from database " << std::endl;
    cc::utils::Result<std::vector<cc::models::Food>> food_items;
    food_items = jsonFoodRepository.list(0, 40);
    if (food_items) {
        std::cout << "food items are found" << std::endl;
        for (auto item : food_items.unwrap()) {
            std::cout << item.to_string() << std::endl;
        }
    } else {
        std::cout << founded_food.unwrap_error().message << std::endl;
    }

    // ############ get by id from data base(json for now ) ##########
    std::cout << "testing remove item from data base" << std::endl;
    cc::utils::Result<void> r = jsonFoodRepository.remove("5449000227041");

    // ############ update or insert food item ##########
    std::cout << "testing update or insert" << std::endl;
    // // 2) Build a Food
    cc::models::Food f;
    f.setId("7622210449283");
    f.setName("Granola Bar");
    f.setBrand(std::string("FooBrand"));
    f.setBarcode(std::string("7622210449283"));
    f.setCaloriesPer100g(420.0);
    f.setServingSizeG(40.0); // default serving size 40g
    f.setSource(cc::models::SOURCE::Manual);
    f.setImageUrl(std::string("https://example.com/granola.jpg"));
    cc::utils::Result<void> r_update_insert = jsonFoodRepository.upsert(f);

    // ############ item Food Service ##########
    //
    // ############ getOrFetchByBarcode ##########
    std::cout << "testing getBy_id_or_Barcode" << std::endl;
    cc::utils::Result<cc::models::Food> returned_food;
    cc::services::FoodService f_service(
        std::make_shared<cc::storage::JsonFoodRepository>(
            "/home/anas/personal_projects/calorie-counter-backend/json_data_base.json"),
        std::make_shared<cc::clients::OpenFoodFactsClient>());
    f_service.getOrFetchByBarcode("7622210449283");

    // ############ addManualFood ##########
    // 2) Build a Food

    std::cout << "testing addManualFood" << std::endl;
    cc::models::Food new_food;
    new_food.setId("2131654967498");
    new_food.setName("minina");
    new_food.setBrand(std::string("wlad 3icha l3arbi"));
    new_food.setBarcode(std::string("2131654967498"));
    new_food.setCaloriesPer100g(420.0);
    new_food.setServingSizeG(40.0); // default serving size 40g
    new_food.setSource(cc::models::SOURCE::Manual);
    new_food.setImageUrl(std::string("https://example.com/granola.jpg"));
    f_service.addManualFood(new_food);

    // ############ deleteFood ##########
    std::cout << "testing deleteFood" << std::endl;
    f_service.deleteFood(new_food.id());

    // ############ listFood ##########
    std::cout << "list  Food" << std::endl;

    cc::utils::Result<std::vector<cc::models::Food>> food_items_2;
    food_items_2 = f_service.listFoods();
    ;
    if (food_items_2) {
        std::cout << "food items are found" << std::endl;
        for (auto item : food_items_2.unwrap()) {
            std::cout << item.to_string() << std::endl;
        }
    } else {
        std::cout << founded_food.unwrap_error().message << std::endl;
    }

    // ############ deleteFood ##########
    cc::models::MealLog breakfast(cc::models::MEALNAME::Breakfast);
    breakfast.addFoodItem(new_food.id(), 100);

    // ############ Meal ##########
    // 2) Build a Meal

    std::cout << "creating new meal" << std::endl;
    cc::models::MealLog new_meal(cc::models::MEALNAME::Breakfast);
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(cc::models::SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    new_meal.addFoodItem(new_food.id(), 100);

    // ############ clear ##########
    // std::cout << "testing clear database " << std::endl;
    // r = jsonFoodRepository.clear();
    // if (food_items)
    // {
    // std::cout<<"food items are found"<<std::endl;
    // for(auto item:food_items.unwrap()){
    // std::cout<<item.to_string()<<std::endl;
    // }
    // }
    // else
    // {
    // std::cout << founded_food.unwrap_error().message << std::endl;
    // }

    // if (r)
    // {
    // std::cout << founded_food.unwrap().to_string() << std::endl;
    // }
    // else
    // {
    // std::cout << founded_food.unwrap_error().message << std::endl;
    // }

    // ################################# test result #############
    // auto r1 = cc::utils::Result<int>::ok(123);
    // if (r1)
    // {
    // int x = r1.unwrap();
    // std::cout << "x : " << x << std::endl;
    // }

    // auto r2 = cc::utils::Result<int>::fail(cc::utils::ErrorCode::ParseError, "bad json");
    // if (!r2)
    // {
    // auto &e = r2.unwrap_error();
    // }

    // auto r3 = cc::utils::Result<void>::ok();
    // auto r4 = cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError, "disk full");
    //////////////////////////////////////////////////////////////////

    /*
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello, Crow!";
    });

    CROW_ROUTE(app, "/json")([](){
        crow::json::wvalue x;
        x["message"] = "Hello, JSON!";
        return x;
    });

    app.port(8080).multithreaded().run();
    */

    // cc::api::Server s(8080, std::make_shared<cc::services::FoodService>(f_service));
    // s.run();
    return 0;
}
