#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <magic_enum.hpp>
namespace cc::models {

enum class NutrientType{Protein,Carbs,Fat,Unknown};
class Nutrient {
  private:
    NutrientType type_;      
    double value_{0.0};     // per 100g basis
    std::string unit_{"g"};

  public:
    // constructors
    Nutrient(NutrientType type, double value, std::string unit = "g");
    Nutrient() = default;
    // getters
    const NutrientType& type() const;
    const std::string& unit() const;
    double value() const;
    // setters
    void setType(NutrientType type);
    void setValue(double v);
    void setUnit(std::string u);
    // to string
    std::string to_string() const;

}; // Nutrient
   // Nutrient serializer
inline void to_json(nlohmann::json& j, const cc::models::Nutrient& n) {
    j = {{"type", magic_enum::enum_name(n.type())}, {"value", n.value()}, {"unit", n.unit()}};
}
inline void from_json(const nlohmann::json& j, cc::models::Nutrient& n) {
    if(j.contains("type")&&!j.at("type").is_null()){
        n.setType(magic_enum::enum_cast<NutrientType>(j.at("type").get<std::string>()).value_or(NutrientType::Unknown));
    }else{
        n.setType(NutrientType::Unknown);
    }
    n.setValue(j.at("value").get<double>());
    n.setUnit(j.at("unit").get<std::string>());
}
} // namespace cc::models
