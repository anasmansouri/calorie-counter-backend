#include "models/food.hpp"
#include "models/nutrient.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace cc {
namespace models {

Food::Food(std::string id_, std::string name_, double caloriesPer100g_,
           std::vector<Nutrient> nutrient_, 
           std::optional<std::string> barcode_, std::optional<std::string> brand_,
           std::optional<std::string> imageUrl_)
    : id_{id_}, name_{name_}, caloriesPer100g_{caloriesPer100g_}, nutrients_{nutrient_},
     barcode_{barcode_}, brand_{brand_}, imageUrl_{imageUrl_} {
}

std::string Food::to_string() const {
    return std::format("{} : {}", this->name_, this->id_);
}
void Food::setName(std::string n) {
    this->name_ = n;
}
const std::string& Food::name() const {
    return this->name_;
}

const std::string& Food::id() const {
    return this->id_;
}
void Food::setId(std::string id) {
    this->id_ = id;
}

const std::optional<std::string>& Food::brand() const {
    return this->brand_;
}
void Food::setBrand(std::optional<std::string> b) {
    this->brand_ = b;
}

const std::optional<std::string>& Food::barcode() const {
    return this->barcode_;
}
void Food::setBarcode(std::optional<std::string> b) {
    this->barcode_ = b;
}

double Food::caloriesPer100g() const {
    return this->caloriesPer100g_;
}

void Food::setCaloriesPer100g(double kcal) {
    this->caloriesPer100g_ = kcal;
}

const std::vector<Nutrient>& Food::nutrients() const {
    return this->nutrients_;
}

void Food::setNutrients(std::vector<Nutrient> ns) {
    this->nutrients_ = ns;
}

const std::optional<std::string>& Food::imageUrl() const {
    return this->imageUrl_;
}
void Food::setImageUrl(std::optional<std::string> url) {
    this->imageUrl_ = url;
}

const SOURCE& Food::source() const {
    return this->source_;
}
void Food::setSource(SOURCE s) {
    this->source_ = s;
}

double Food::totalKcal(double servingSizeG) const {
    return (servingSizeG) * (this->caloriesPer100g_)/100;
}
} // namespace models
} // namespace cc
