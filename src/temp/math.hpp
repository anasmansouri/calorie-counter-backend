#pragma once
#include <numeric>
#include <stdexcept>
#include <vector>

// A few simple math utilities for testing GoogleTest
namespace cc::temp {

/// Adds two integers
inline int add(int a, int b) {
    return a + b;
}

/// Multiplies two integers
inline int multiply(int a, int b) {
    return a * b;
}

/// Divides two integers, throws if division by zero
inline double divide(int a, int b) {
    if (b == 0)
        throw std::invalid_argument("division by zero");
    return static_cast<double>(a) / b;
}

/// Computes the average of a vector of numbers
inline double average(const std::vector<int>& values) {
    if (values.empty())
        throw std::invalid_argument("empty vector");
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / static_cast<double>(values.size());
}

} // namespace cc::temp
