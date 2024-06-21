#pragma once

#include <vector>
#include <functional>
#include <string>
#include <print>
#include <cassert>
#include <source_location>

#define ASSERT_EQ(leftValue, rightValue) \
    if ( leftValue != rightValue ) { \
        auto location = std::source_location::current(); \
        std::println("Assert failed: {}(which is equal to {}) is not equal to {} at {}:{}", #leftValue, leftValue, #rightValue, location.file_name(), location.line()); \
        throw std::invalid_argument("Assertion failed"); \
    }

#define ASSERT_NEQ(leftValue, rightValue) \
    if ( leftValue == rightValue ) { \
        auto location = std::source_location::current(); \
        std::println("Assert failed: {} is equal to {} at {}:{}", #leftValue, #rightValue, location.file_name(), location.line()); \
        throw std::invalid_argument("Assertion failed"); \
    }

class UnitTests {
public:
    using TestFunc = std::function<void(void)>;

    void addTest(const std::string& name, TestFunc&& func);

    void run();

private:
    std::vector<std::pair<std::string, TestFunc>> tests;
};
