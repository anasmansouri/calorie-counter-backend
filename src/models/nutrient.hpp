#pragma once
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
namespace cc::models {

class Nutrient {
  private:
    std::string name_;      // Protein , Carbs , Fat
    double value_{0.0};     // per 100g basis
    std::string unit_{"g"}; // eg. "g", "mg"

  public:
    // constructors
    Nutrient(std::string name, double value, std::string unit = "g");
    Nutrient() = default;
    // getters
    const std::string& name() const;
    const std::string& unit() const;
    double value() const;
    // setters
    void setName(std::string n);
    void setValue(double v);
    void setUnit(std::string u);
    // to string
    std::string to_string() const;

}; // Nutrient
   // Nutrient serializer
inline void to_json(nlohmann::json& j, const cc::models::Nutrient& n) {
    j = {{"name", n.name()}, {"value", n.value()}, {"unit", n.unit()}};
}
inline void from_json(const nlohmann::json& j, cc::models::Nutrient& n) {
    n.setName(j.at("name").get<std::string>());
    n.setValue(j.at("value").get<double>());
    n.setUnit(j.at("unit").get<std::string>());
}
} // namespace cc::models
