#include <functional>
#include <iostream>
#include <array>

#include <aoc/solution_1_1.cpp>
#include <aoc/solution_1_2.cpp>


namespace aoc
{

// ----------------------------------------------

#define R(day) { solve_##day##_1, solve_##day##_2 }

const std::array<std::array<std::function<void()>, 2>, 25> days {{
    R(1),
}};

void solve_day(usize day)
{
    const auto d = days[day-1];
    std::cerr << "----------[ Day " << day << " ]----------" << "\n";
    std::cerr << ":: Part 1 ::\n";
    if (d[0]) { d[0](); }
    std::cerr << "\n:: Part 2 ::\n";
    if (d[1]) { d[1](); }
    std::cerr << "----------------------------" << "\n";
}

}

// ----------------------------------------------

int main()
{
    constexpr usize day = 1;

    std::cout << "Advent of Code - 2023" << std::endl;
    aoc::solve_day(day);

    return 0;
}

// ----------------------------------------------
