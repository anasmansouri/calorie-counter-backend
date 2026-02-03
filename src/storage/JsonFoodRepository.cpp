#include "storage/JsonFoodRepository.hpp"

namespace cc::storage {
JsonFoodRepository::JsonFoodRepository(std::string filePath)
    : filePath_{filePath} {}
cc::utils::Result<void> JsonFoodRepository::save(const cc::models::Food &food) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
  } else {
    file_content = nlohmann::json::array();
  }
  for (auto item : file_content) {
    if (item["id"] == food.id()) {
      // no need to save this food again , because there is only one barcode per
      // food
      return cc::utils::Result<void>::ok();
    }
  }
  file_content.push_back(food);
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

cc::utils::Result<cc::models::Food>
JsonFoodRepository::getById_or_Barcode(const std::string &id) {

  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    for (auto i : file_content) {
      if (i["id"].get<std::string>() == id) {
        return cc::utils::Result<cc::models::Food>::ok(cc::models::Food(i));
      }
    }
    return cc::utils::Result<cc::models::Food>::fail(
        cc::utils::ErrorCode::NotFound, "item not found");
  } else {
    return cc::utils::Result<cc::models::Food>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}

cc::utils::Result<std::vector<cc::models::Food>>
JsonFoodRepository::list(int offset, int limit) {
  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    std::vector<cc::models::Food> food_vector;
    for (int i = offset; i < file_content.size() && i < offset+limit; i++) {
      food_vector.push_back(cc::models::Food(file_content[i]));
    }
    return cc::utils::Result<std::vector<cc::models::Food>>::ok(food_vector);
  } else {
    return cc::utils::Result<std::vector<cc::models::Food>>::fail(
        cc::utils::ErrorCode::NotFound,
        "file is empty , or can't open that file");
  }
}
cc::utils::Result<void> JsonFoodRepository::remove(const std::string &id) {

  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    for (int i = 0; i < file_content.size(); i++) {
      if (file_content[i]["id"].get<std::string>() == id) {
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
cc::utils::Result<void>
JsonFoodRepository::upsert(const cc::models::Food &food) {

  std::lock_guard<std::mutex> lock(this->mtx_);
  std::ifstream infile(this->filePath_);
  nlohmann::json file_content;
  if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
    infile >> file_content;
    infile.close();
    bool item_updated = false;
    for (int i = 0; i < file_content.size(); i++) {
      if (file_content[i]["id"].get<std::string>() == food.id()) {
        file_content[i] = food;
        item_updated = true;
      }
    }
    if (!item_updated) {
      file_content.push_back(food);
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
cc::utils::Result<void> JsonFoodRepository::clear() {
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

void JsonFoodRepository::setFlushOnWrite(bool enable) {}
} // namespace cc::storage
