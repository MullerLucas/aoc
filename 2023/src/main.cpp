#include <chrono>
#include <functional>
#include <iostream>
#include <array>

#include <aoc/day_1.cpp>
#include <aoc/day_2.cpp>


namespace aoc
{

// ----------------------------------------------

#define R(day) { solve_##day##_1, solve_##day##_2 }

const std::array<std::array<std::function<void()>, 2>, 25> days {{
    { solve_1_1, solve_1_2 },
    { solve_day_2, nullptr },
}};

void solve_day(usize day)
{
    const auto d = days[day-1];
    std::cout << "Advent of Code - 2023 - Day " << day << std::endl;

    std::cerr << "\n---------- Part 1 ----------\n\n";
    const auto t1 = std::chrono::high_resolution_clock::now();
    if (d[0]) { d[0](); }
    const auto t2 = std::chrono::high_resolution_clock::now();
    std::cerr << "\n----------------------------\n";
    std::cerr << "==> Timing: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " us\n";

    std::cerr << "\n---------- Part 2 ----------\n\n";
    const auto t3 = std::chrono::high_resolution_clock::now();
    if (d[1]) { d[1](); }
    const auto t4 = std::chrono::high_resolution_clock::now();
    std::cerr << "\n----------------------------\n";
    std::cerr << "==> Timing: " << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << " us\n";
}

}

// ----------------------------------------------

int main()
{
    constexpr usize day = 2;

    aoc::solve_day(day);

    return 0;
}

// ----------------------------------------------
