#include <chrono>
#include <functional>
#include <iostream>
#include <array>
#include <hell/common.h>
#include <fstream>
#include <cassert>


namespace aoc
{

// ----------------------------------------------

i64 solve_1_1(std::ifstream&& file);
i64 solve_1_2(std::ifstream&& file);
i64 solve_2_1(std::ifstream&& file);
i64 solve_2_2(std::ifstream&& file);
i64 solve_3_1(std::ifstream&& file);
i64 solve_3_2(std::ifstream&& file);
i64 solve_4_1(std::ifstream&& file);
i64 solve_4_2(std::ifstream&& file);
i64 solve_5_1(std::ifstream&& file);

// ----------------------------------------------

#define R(day) { solve_##day##_1, solve_##day##_2 }

const std::array<std::array<std::function<i64(std::ifstream&&)>, 2>, 25> days {{
    { solve_1_1, solve_1_2 },
    { solve_2_1, solve_2_2 },
    { solve_3_1, solve_3_2 },
    { solve_4_1, solve_4_2 },
    { solve_5_1, nullptr },
}};

void solve_day(usize day, bool use_example)
{
    const auto d = days[day-1];
    std::cout << "Advent of Code - 2023 - Day " << day << std::endl;

    char buf[255];
    const char* ident = use_example ? "example" : "input";

    for (usize part = 0; part < 2; ++part) {
        usize len = std::sprintf(buf, "resources/%s_%zu_%zu.txt", ident, day, part+1);
        assert(len > 0);
        std::ifstream file;
        file.open(buf);

        std::cerr << "\n---------- Part " << part+1 <<" ----------\n\n";
        const auto t1 = std::chrono::high_resolution_clock::now();

        if (d[part]) {
            const auto result = d[part](std::move(file));
            std::cout << result << std::endl;
        }

        const auto t2 = std::chrono::high_resolution_clock::now();
        std::cerr << "\n----------------------------\n";
        std::cerr << "==> Timing: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " us\n";
    }
}

}

// ----------------------------------------------

int main()
{
    aoc::solve_day(5, false);
    return 0;
}

// ----------------------------------------------
