#include "storage/JsonMealRepository.hpp"

#include <bits/chrono.h>

#include <algorithm>
#include <cmath>
#include <string>
#include <string_view>

#include "models/meal_log.hpp"
#include "utils/date_time_utils.hpp"

namespace cc::storage {
JsonMealRepository::JsonMealRepository(std::string filePath)
    : filePath_{filePath} {
  this->sync_meals_id();
}

cc::utils::Result<void> JsonMealRepository::sync_meals_id() {
  int max_id = cc::models::MealLog::next_id_;
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    for (auto i : file_content) {
      if (i.contains("id")) {
        max_id = std::max<int>(max_id, i["id"].get<int>());
      }
    }
    cc::models::MealLog::next_id_ = max_id;
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
};

cc::utils::Result<void> JsonMealRepository::save(
    const cc::models::MealLog& meal) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
  } else {
    file_content = nlohmann::json::array();
  }
  file_content.push_back(meal);
  std::cout << file_content.dump(4) << std::endl;
  std::ofstream o(filePath_);
  if (o.is_open()) {
    o << file_content.dump(4) << std::endl;
    o.close();
    return cc::utils::Result<void>::ok();
  } else {
    std::cout << "can't open file" + filePath_ << std::endl;
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't open file");
  }
}

cc::utils::Result<cc::models::MealLog> JsonMealRepository::getById(
    const int id) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    for (auto i : file_content) {
      if (i["id"].get<int>() == id) {
        cc::models::MealLog meal(i);
        return cc::utils::Result<cc::models::MealLog>::ok(meal);
      }
    }
    return cc::utils::Result<cc::models::MealLog>::fail(
        cc::utils::ErrorCode::NotFound, "item not found");
  } else {
    return cc::utils::Result<cc::models::MealLog>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>>
JsonMealRepository::getByDate(std::chrono::system_clock::time_point tsUtc) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    std::vector<cc::models::MealLog> meals_vector;
    for (int i = 0; i < file_content.size(); i++) {
      if (file_content[i].contains("tsUtc")) {
        if (floor<std::chrono::days>(cc::utils::fromIso8601(
                file_content[i]["tsUtc"].get<std::string>())) ==
            floor<std::chrono::days>(tsUtc)) {
          meals_vector.push_back(cc::models::MealLog(file_content[i]));
        }
      }
    }
    return cc::utils::Result<std::vector<cc::models::MealLog>>::ok(
        meals_vector);
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>>
JsonMealRepository::getByName(cc::models::MEALNAME name) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    std::vector<cc::models::MealLog> meals_vector;
    std::string_view searched_name = magic_enum::enum_name(name);
    for (int i = 0; i < file_content.size(); i++) {
      if (file_content[i].contains("name")) {
        if (file_content[i]["name"].get<std::string>() == searched_name) {
          meals_vector.push_back(cc::models::MealLog(file_content[i]));
        }
      }
    }
    return cc::utils::Result<std::vector<cc::models::MealLog>>::ok(
        meals_vector);
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}

cc::utils::Result<std::vector<cc::models::MealLog>> JsonMealRepository::list(
    int offset, int limit) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  std::vector<cc::models::MealLog> meals_vector{};

  if (infile.is_open()) {
    if (infile.peek() == std::ifstream::traits_type::eof()) {
      return cc::utils::Result<std::vector<cc::models::MealLog>>::ok(
          meals_vector);
    } else {
      infile >> file_content;
      infile.close();
      for (int i = offset; i < file_content.size() && i < offset + limit; i++) {
        meals_vector.push_back(cc::models::MealLog(file_content[i]));
      }
      return cc::utils::Result<std::vector<cc::models::MealLog>>::ok(
          meals_vector);
    }
  } else {
    return cc::utils::Result<std::vector<cc::models::MealLog>>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}
cc::utils::Result<void> JsonMealRepository::remove(const int id) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    for (int i = 0; i < file_content.size(); i++) {
      if (file_content[i]["id"].get<int>() == id) {
        file_content.erase(i);
        std::ofstream o(this->filePath_);
        if (o.is_open()) {
          o.clear();
          o << file_content.dump(4) << std::endl;
          o.close();
          return cc::utils::Result<void>::ok();
        } else {
          return cc::utils::Result<void>::fail(
              cc::utils::ErrorCode::StorageError, "can't remove item");
        }
      }
    }
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::NotFound,
                                         "item not found");
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::NotFound,
                                         "item not found");
  }
  return cc::utils::Result<void>::fail(cc::utils::ErrorCode::NotFound,
                                       "item not found");
}

// update or insert if doesn't exist
cc::utils::Result<void> JsonMealRepository::upsert(
    const cc::models::MealLog& meal) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    bool item_updated = false;
    for (int i = 0; i < file_content.size(); i++) {
      // pay attention to this comparison
      if (file_content[i]["id"].get<int>() == meal.id()) {
        file_content[i] = meal;
        item_updated = true;
      }
    }
    if (!item_updated) {
      file_content.push_back(meal);
    }
    std::ofstream o(this->filePath_);
    if (o.is_open()) {
      o.clear();
      o << file_content.dump(4) << std::endl;
      o.close();
      return cc::utils::Result<void>::ok();
    } else {
      return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                           "can't update or insert item");
    }
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't open file");
  }
}

// clear all records
cc::utils::Result<void> JsonMealRepository::clear() {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ofstream o(this->filePath_);
  if (o.is_open()) {
    o.clear();

    nlohmann::json empty_json = nlohmann::json::array();
    o << empty_json.dump(4) << std::endl;
    o.close();
    return cc::utils::Result<void>::ok();
  } else {
    return cc::utils::Result<void>::fail(cc::utils::ErrorCode::StorageError,
                                         "can't remove item");
  }
}

}  // namespace cc::storage
