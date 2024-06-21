#include "UnitTests.hpp"

void UnitTests::addTest(const std::string& name, TestFunc&& func) {
    tests.push_back({name, std::move(func)});
}

void UnitTests::run()
{
    constexpr auto greenColor = "\033[32m";
    constexpr auto redColor = "\033[31m";
    constexpr auto endColor = "\033[m";

    size_t totalCount = 0;
    size_t successCount = 0;

    for (auto &[name, testFunc] : tests) {
        if (testFunc) {
            std::println("Executing test {}...", name);
            totalCount++;
            try {
                testFunc();
                std::println("{} {}PASSED{}.", name, greenColor, endColor);
                successCount++;
            }
            catch (std::exception &e) {
                std::println("{} {}FAILED{}.", name, redColor, endColor);
            }
        }
    }
    std::println("{} of {} {} have passed", successCount, totalCount, totalCount == 1 ? "test" : "tests");
}