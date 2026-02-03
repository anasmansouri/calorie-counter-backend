#include "Server.hpp"

#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <magic_enum.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "nlohmann/json.hpp"
#include "utils/Result.hpp"
#include "utils/date_time_utils.hpp"

namespace cc {
namespace api {

Server::Server(int port, std::shared_ptr<cc::services::FoodService> foodService,
               std::shared_ptr<cc::services::MealService> mealService)
    : port_{port}, foodService_{foodService}, mealService_{mealService} {}
Server::~Server() { this->stop(); }
void Server::setupRoutes() {
  CROW_ROUTE(this->app, "/").methods(crow::HTTPMethod::Get)([]() {
    return "Hello, Crow!";
  });
  CROW_ROUTE(this->app, "/health").methods(crow::HTTPMethod::Get)([]() {
    crow::json::wvalue j;
    j["status"] = "ok";
    return crow::response{j};
  });

  CROW_ROUTE(this->app, "/foods")
      .methods(crow::HTTPMethod::Get)([this](const crow::request& req) {
        auto offset = req.url_params.get("offset");
        auto limit = req.url_params.get("limit");
        int offset_value = offset ? std::atoi(offset) : 0;
        int limit_value = limit ? std::atoi(limit) : 50;
        cc::utils::Result<std::vector<cc::models::Food>> list_of_food =
            this->foodService_->listFoods(offset_value, limit_value);
        crow::json::wvalue response_json;
        if (list_of_food) {
          nlohmann::json list_of_food_json = list_of_food.unwrap();
          response_json = cc::utils::to_crow_json(list_of_food_json);
          return crow::response(200, response_json);
        } else {
          response_json["error"] = list_of_food.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  list_of_food.unwrap_error().code),
              response_json);
        }
      });
  CROW_ROUTE(this->app, "/search_by_barcode")
      .methods(crow::HTTPMethod::Get)([this](const crow::request& req) {
        auto barcode = req.url_params.get("barcode");
        crow::json::wvalue response_json;
        // test this
        if (barcode == nullptr || std::string(barcode).empty()) {
          response_json["error"] = "missed barcode";
          return crow::response(404, response_json);
        }
        cc::utils::Result<cc::models::Food> founded_food =
            this->foodService_->getOrFetchByBarcode(barcode);

        if (founded_food) {
          nlohmann::json founded_food_json = founded_food.unwrap();
          response_json = cc::utils::to_crow_json(founded_food_json);
          return crow::response(200, response_json);
        } else {
          response_json["error"] = founded_food.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  founded_food.unwrap_error().code),
              response_json);
        }
      });

  CROW_ROUTE(this->app, "/foods")
      .methods(crow::HTTPMethod::DELETE)([this](const crow::request& req) {
        auto barcode = req.url_params.get("barcode");
        cc::utils::Result<void> result;
        result = this->foodService_->deleteFood(barcode);
        crow::json::wvalue response_json;
        if (result) {
          response_json["status"] = "ok";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = result.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  result.unwrap_error().code),
              response_json);
        }
      });

  CROW_ROUTE(this->app, "/foods")
      .methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
          return crow::response(400, "invalid Json body");
        }

        // #todo  no nutrition where added
        cc::models::Food new_food;
        new_food.setId(body["id"].s());
        new_food.setName(body["name"].s());
        new_food.setBrand(body["brand"].s());
        new_food.setBarcode(body["barcode"].s());
        new_food.setCaloriesPer100g(body["caloriePer100g"].d());
        new_food.setServingSizeG(
            body["servingSizeG"].d());  // default serving size 40g
        new_food.setSource(cc::models::SOURCE::Manual);
        // new_food.setNutrients({{"Protein", 24, "g"}, {"Carbs", 100, "g"}});
        //  nutrition is optional
        std::vector<cc::models::Nutrient> nutrients;
        if (body.has("nutrient")) {
          for (const auto& n : body["nutrient"]) {
            // Convert Crow json node -> string -> nlohmann::json -> Nutrient
            cc::models::Nutrient nutrient;
            nutrient.setName(n["name"].s());
            nutrient.setUnit(n["unit"].s());
            nutrient.setValue(n["value"].d());
            nutrients.push_back(nutrient);
          }
        }
        new_food.setNutrients(nutrients);

        // new_food.setImageUrl(std::string("https://example.com/granola.jpg"));
        auto result = this->foodService_->addManualFood(new_food);
        crow::json::wvalue response_json;
        if (result) {
          response_json["status"] = "item was added";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = result.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  result.unwrap_error().code),
              response_json);
        }
      });
  CROW_ROUTE(this->app, "/foods")
      .methods(crow::HTTPMethod::PUT)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
          return crow::response(400, "invalid Json body");
        }

        cc::models::Food new_food;
        new_food.setId(body["id"].s());
        new_food.setName(body["name"].s());
        new_food.setBrand(body["brand"].s());
        new_food.setBarcode(body["barcode"].s());
        new_food.setCaloriesPer100g(body["caloriePer100g"].d());
        new_food.setServingSizeG(
            body["servingSizeG"].d());  // default serving size 40g
        new_food.setSource(cc::models::SOURCE::Manual);
        std::vector<cc::models::Nutrient> nutrients;
        if (body.has("nutrient")) {
          for (const auto& n : body["nutrient"]) {
            // Convert Crow json node -> string -> nlohmann::json -> Nutrient
            cc::models::Nutrient nutrient;
            nutrient.setName(n["name"].s());
            nutrient.setUnit(n["unit"].s());
            nutrient.setValue(n["value"].d());
            nutrients.push_back(nutrient);
          }
        }
        new_food.setNutrients(nutrients);
        // new_food.setImageUrl(std::string("https://example.com/granola.jpg"));
        auto result = this->foodService_->updateFood(new_food);
        crow::json::wvalue response_json;
        if (result) {
          response_json["status"] = "item was updated successfuly";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = result.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  result.unwrap_error().code),
              response_json);
        }
      });

  ///////////////////////Meals//////////////////////////

  // GET /meals?offset=0&limit=50
  CROW_ROUTE(this->app, "/meals")
      .methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto offset = req.url_params.get("offset");
        auto limit = req.url_params.get("limit");

        int offset_value = offset ? std::atoi(offset) : 0;
        int limit_value = limit ? std::atoi(limit) : 50;

        auto res = this->mealService_->listMeals(offset_value, limit_value);

        crow::json::wvalue response_json;
        if (res) {
          nlohmann::json j = res.unwrap();  // vector<MealLog> -> json (to_json)
          response_json = cc::utils::to_crow_json(j);
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });

 
  
  // GET /meals/by_name?name=Lunch
  CROW_ROUTE(this->app, "/meals/by_name")
      .methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto name = req.url_params.get("name");
        crow::json::wvalue response_json;

        if (name == nullptr || std::string(name).empty()) {
          response_json["error"] = "missing name";
          return crow::response(400, response_json);
        }

        auto res = this->mealService_->getByName(std::string(name));

        if (res) {
          nlohmann::json j = res.unwrap();
          response_json = cc::utils::to_crow_json(j);
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });

  
  // GET /meals/by_date?day=2&month=2&year=2026
  CROW_ROUTE(this->app, "/meals/by_date")
      .methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto d = req.url_params.get("day");
        auto m = req.url_params.get("month");
        auto y = req.url_params.get("year");

        crow::json::wvalue response_json;

        if (!d || !m || !y) {
          response_json["error"] = "missing day/month/year";
          return crow::response(400, response_json);
        }

        int day = std::atoi(d);
        int month = std::atoi(m);
        int year = std::atoi(y);

        auto res = this->mealService_->getByDate(day, month, year);

        if (res) {
          nlohmann::json j = res.unwrap();
          response_json = cc::utils::to_crow_json(j);
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });

  // POST /meals
  // Body example:
  // {
  //   "name": "Lunch",
  //   "tsUtc": "2026-02-02T13:05:00Z",   // optional
  //   "foodItems": [ {"foodId":"123", "grams":40.0} ] // optional
  // }
  CROW_ROUTE(this->app, "/meals")
      .methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid Json body");

        cc::models::MealLog meal;

        // name (required)
        if (!body.has("name"))
          return crow::response(400, "missing field: name");
        const std::string nameStr = body["name"].s();

        auto meal_name = magic_enum::enum_cast<cc::models::MEALNAME>(nameStr);
        if (!meal_name) return crow::response(400, "invalid meal name");
        meal.setName(*meal_name);

        // tsUtc (optional) else now()
        if (body.has("tsUtc")) {
          meal.setTime(cc::utils::fromIso8601(std::string(body["tsUtc"].s())));
        } else {
          meal.setTime(std::chrono::system_clock::now());
        }

        // foodItems (optional)
        if (body.has("foodItems")) {
          std::vector<std::pair<std::string, double>> items;
          for (const auto& it : body["foodItems"]) {
            // Expect { "foodId": "...", "grams": 12.3 }
            if (it.has("foodId") && it.has("grams")) {
              items.emplace_back(std::string(it["foodId"].s()),
                                 it["grams"].d());
            }
          }
          meal.setFoodItems(items);
        }

        auto res = this->mealService_->addNewMeal(meal);

        crow::json::wvalue response_json;
        if (res) {
          response_json["status"] = "item was added";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });
  

  // PUT /meals  (update)
  // Body example:
  // {
  //   "id": 10,
  //   "name": "Dinner",
  //   "tsUtc": "2026-02-02T19:00:00Z",
  //   "foodItems": [ {"foodId":"123","grams":50.0} ]
  // }
  CROW_ROUTE(this->app, "/meals")
      .methods(crow::HTTPMethod::PUT)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid Json body");

        if (!body.has("id")) return crow::response(400, "missing field: id");
        if (!body.has("name"))
          return crow::response(400, "missing field: name");

        cc::models::MealLog meal;
        meal.setId(body["id"].i());

        const std::string nameStr = body["name"].s();
        auto meal_name = magic_enum::enum_cast<cc::models::MEALNAME>(nameStr);
        if (!meal_name) return crow::response(400, "invalid meal name");
        meal.setName(*meal_name);

        if (body.has("tsUtc")) {
          meal.setTime(cc::utils::fromIso8601(std::string(body["tsUtc"].s())));
        } else {
          meal.setTime(std::chrono::system_clock::now());
        }

        if (body.has("foodItems")) {
          std::vector<std::pair<std::string, double>> items;
          for(auto item : body["foodItems"]){
            items.push_back({item[0].s(),item[1].d()});
          }
          meal.setFoodItems(items);
        }

        auto res = this->mealService_->updateMeal(meal);

        crow::json::wvalue response_json;
        if (res) {
          response_json["status"] = "item was updated successfully";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });
  
  // DELETE /meals?id=10
  CROW_ROUTE(this->app, "/meals")
      .methods(crow::HTTPMethod::DELETE)([this](const crow::request& req) {
        auto idp = req.url_params.get("id");
        crow::json::wvalue response_json;

        if (!idp) {
          response_json["error"] = "missing id";
          return crow::response(400, response_json);
        }

        int id = std::atoi(idp);
        auto res = this->mealService_->deleteMeal(id);

        if (res) {
          response_json["status"] = "ok";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });

  // OPTIONAL: clear all meals
  CROW_ROUTE(this->app, "/meals/clear")
      .methods(crow::HTTPMethod::DELETE)([this](const crow::request&) {
        auto res = this->mealService_->clear_data_base();
        crow::json::wvalue response_json;

        if (res) {
          response_json["status"] = "ok";
          return crow::response(200, response_json);
        } else {
          response_json["error"] = res.unwrap_error().message;
          return crow::response(
              cc::utils::convert_error_code_into_HTTP_Responses(
                  res.unwrap_error().code),
              response_json);
        }
      });
  

  ///////////////////////Meals//////////////////////////
}
void Server::start() {
  std::call_once(this->once, [this] { this->setupRoutes(); });
  // if(this->server_thread.joinable()){return;}

  try {
    this->server_thread = std::thread([this] {
      {
        std::lock_guard<std::mutex> lk(m_);
        running_ = true;
      }
      cv_.notify_one();
      this->app.port(this->port_).multithreaded().run();
    });
    // wait until thread reached the "running" point
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { return running_; });
    std::this_thread::sleep_for(std::chrono::seconds(2));
  } catch (std::exception& e) {
    std::cerr << "Server thread exception" << e.what() << std::endl;
    std::terminate();
  }
}
void Server::stop() {
  std::cout << "destructor" << std::endl;
  if (this->server_thread.joinable()) {
    this->app.stop();
    this->server_thread.join();
  }
}
void Server::enableCors(bool enable) {}
}  // namespace api
}  // namespace cc
