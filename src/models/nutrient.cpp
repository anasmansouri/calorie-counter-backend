#pragma once
#include "models/nutrient.hpp"
#include "magic_enum.hpp"
#include <iostream>
#include <string>

namespace cc {
namespace models {

Nutrient::Nutrient(NutrientType type, double value, std::string unit)
    : type_{type}, value_{value}, unit_{unit} {
}

const NutrientType& Nutrient::type() const {
    return this->type_;
}
void Nutrient::setType(NutrientType type) {
    this->type_ = type;
}

double Nutrient::value() const {
    return this->value_;
}
void Nutrient::setValue(double v) {
    this->value_ = v;
}

const std::string& Nutrient::unit() const {
    return this->unit_;
}

void Nutrient::setUnit(std::string u) {
    this->unit_ = u;
}

std::string Nutrient::to_string() const {
    return std::format("{} : {} : {}", magic_enum::enum_name(this->type_), this->value_, this->unit_);
}
} // namespace models
} // namespace cc
