#include <array>
#include <cassert>
#include <fstream>
#include <string>
#include <iostream>
#include <optional>

#include <hell/common.h>
#include <string_view>

namespace aoc
{

static const std::array<std::array<std::string_view, 2>, 10> PATTERNS = {{
    {"0", "zero"},
    {"1", "one"},
    {"2", "two"},
    {"3", "three"},
    {"4", "four"},
    {"5", "five"},
    {"6", "six"},
    {"7", "seven"},
    {"8", "eight"},
    {"9", "nine"},
}};

// ----------------------------------------------

static std::optional<i32> match_pattern(const std::string& line, usize start)
{
    for (usize i = 0; i < PATTERNS.size(); ++i) {
        for (const auto& pat : PATTERNS[i]) {
            if ((line.size() - start) < pat.size()) { continue; }

            bool matches = true;
            for (usize c = 0; c < pat.size(); ++c) {
                if (line[start + c] != pat[c]) {
                    matches = false;
                    break;
                }
            }

            if (matches) {
                return i;
            }
        }
    }

    return std::nullopt;
}

// ----------------------------------------------

void solve_1_2() {
    std::ifstream file;
    file.open("resources/input_1_1.txt");
    assert(file.is_open() && "Failed to open file");

    std::string line;
    i32 sum = 0;

    while (std::getline(file, line)) {
        if (line.size() < 2) { continue; }

        std::optional<i32> left_val = std::nullopt;
        for (usize i = 0; i < line.size(); ++i) {
            left_val = match_pattern(line, i);
            if (left_val.has_value()) {
                break;
            }
        }

        std::optional<i32> right_val = std::nullopt;
        for (i64 i = line.size(); i >= 0; --i) {
            right_val = match_pattern(line, i);
            if (right_val.has_value()) {
                break;
            }
        }

        assert(left_val.has_value() && "Left number not found");
        assert(right_val.has_value() && "Right number not found");

        i32 val = *left_val * 10 + *right_val;
        sum += val;
    }

    std::cout << "Sum: " << sum << std::endl;

    file.close();
}

// ----------------------------------------------

}
