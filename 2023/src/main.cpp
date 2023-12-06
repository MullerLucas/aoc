#include <functional>
#include <iostream>
#include <array>

#include <aoc/day_1.cpp>


namespace aoc
{
    const std::array<std::function<void()>, 1> days {
        run_day_1,
    };

    void run_day(usize idx)
    {
        std::cerr << "-------------[Day " << idx + 1 << "]-----------------" << "\n";
        days[idx]();
        std::cerr << "-------------------------------------" << std::endl;
    }
}

int main()
{
    using namespace aoc;

    std::cout << "Advent of Code - 2023" << std::endl;

    for (usize i = 0; i < aoc::days.size(); ++i) {
        aoc::run_day(i);
    }

    return 0;
}
