#include "models/food.hpp"
#include "models/nutrient.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace cc {
namespace models {

Food::Food(std::string id_, std::string name_, double caloriesPer100g_,
           std::vector<Nutrient> nutrient_, std::optional<double> servingSizeG_,
           std::optional<std::string> barcode_, std::optional<std::string> brand_,
           std::optional<std::string> imageUrl_)
    : id_{id_}, name_{name_}, caloriesPer100g_{caloriesPer100g_}, nutrients_{nutrient_},
      servingSizeG_{servingSizeG_}, barcode_{barcode_}, brand_{brand_}, imageUrl_{imageUrl_} {
    // #todo double check this comment , kaybanli masaleh lwalo
    // this->servingSizeG_=0; // mchkok fhadi 3lach servingSizeG intialiazed by 0
}

std::string Food::to_string() const {
    return std::format("{} : {} : {}", this->name_, this->id_, this->servingSizeG_.value());
}
// Name
void Food::setName(std::string n) {
    this->name_ = n;
}
const std::string& Food::name() const {
    return this->name_;
}

// Id
const std::string& Food::id() const {
    return this->id_;
}
void Food::setId(std::string id) {
    this->id_ = id;
}

// brand
const std::optional<std::string>& Food::brand() const {
    return this->brand_;
}
void Food::setBrand(std::optional<std::string> b) {
    this->brand_ = b;
}

// Barcode
const std::optional<std::string>& Food::barcode() const {
    return this->barcode_;
}
void Food::setBarcode(std::optional<std::string> b) {
    this->barcode_ = b;
}

// CaloriesPer100g
double Food::caloriesPer100g() const {
    return this->caloriesPer100g_;
}

void Food::setCaloriesPer100g(double kcal) {
    this->caloriesPer100g_ = kcal;
}

const std::optional<double>& Food::servingSizeG() const {
    return this->servingSizeG_;
}
void Food::setServingSizeG(std::optional<double> g) {
    this->servingSizeG_ = g;
}

// Nutrient

const std::vector<Nutrient>& Food::nutrients() const {
    return this->nutrients_;
}

void Food::setNutrients(std::vector<Nutrient> ns) {
    this->nutrients_ = ns;
}

// media/source
const std::optional<std::string>& Food::imageUrl() const {
    return this->imageUrl_;
}
void Food::setImageUrl(std::optional<std::string> url) {
    this->imageUrl_ = url;
}

// source
const SOURCE& Food::source() const {
    return this->source_;
}
void Food::setSource(SOURCE s) {
    this->source_ = s;
}

// totalKcal
double Food::totalKcal() const {
    return (this->servingSizeG_.value()) * (this->caloriesPer100g_)/100;
}
} // namespace models
} // namespace cc
