#pragma once
#include "models/nutrient.hpp"
#include <iostream>
#include <string>

namespace cc {
namespace models {

Nutrient::Nutrient(std::string name, double value, std::string unit)
    : name_{name}, value_{value}, unit_{unit} {
    std::cout << "Nutrient " << name << " is created" << std::endl;
}

// Name
const std::string& Nutrient::name() const {
    return this->name_;
}
void Nutrient::setName(std::string n) {
    this->name_ = n;
}

// value
double Nutrient::value() const {
    return this->value_;
}
void Nutrient::setValue(double v) {
    this->value_ = v;
}

// Unit
const std::string& Nutrient::unit() const {
    return this->unit_;
}

void Nutrient::setUnit(std::string u) {
    this->unit_ = u;
}

std::string Nutrient::to_string() const {
    return std::format("{} : {} : {}", this->name_, this->value_, this->unit_);
}
} // namespace models
} // namespace cc
