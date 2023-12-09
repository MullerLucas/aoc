#include <cassert>
#include <cctype>
#include <fstream>
#include <string>
#include <iostream>
#include <hell/common.h>
#include <vector>



namespace aoc
{

bool is_symbol(const std::vector<std::string>& schema, i64 x, i64 y)
{
    if (y < 0 || y >= static_cast<i64>(schema.size()))       { return false; }
    if (x < 0 || x >= static_cast<i64>(schema.at(y).size())) { return false; }
    const char c = schema.at(y).at(x);
    return !std::isdigit(c) && (c != '.');
}

bool is_part_number(const std::vector<std::string>& schema, usize x_start,
                    usize y_start, usize len)
{
    i64 x_left  = static_cast<i64>(x_start) - 1;
    i64 x_right = static_cast<i64>(x_start) + len;

    i64 y_to_check[] {
        static_cast<i64>(y_start) - 1,
        static_cast<i64>(y_start) + 1,
    };

    // check below and above
    for (auto y : y_to_check) {
        for (i64 x = x_left; x <= x_right; ++x) {
            if (is_symbol(schema, x, y)) {
                return true;
            }
        }
    }

    // check left and right
    return is_symbol(schema, x_left, y_start) ||
           is_symbol(schema, x_right, y_start);
}

i32 parse_part_numbers(const std::vector<std::string>& schema)
{
    i32 result = 0;

    for (usize y = 0; y < schema.size(); ++y) {
        i32 value = 0;
        usize len = 0;

        for (usize x = 0; x <= schema.at(y).size(); ++x) {
            if (x < schema.at(y).size() && std::isdigit(schema.at(y).at(x))) {
                value = value * 10 + static_cast<i32>(schema.at(y).at(x) - 48);
                ++len;
            } else {
                if (len != 0) {
                    const usize x_start = x - len;

                    if (is_part_number(schema, x_start, y, len)) {
                        result += value;
                    }

                    value = 0;
                    len = 0;
                }
            }
        }
    }

    return result;
}

void solve_3_1()
{
    std::ifstream file;
    // file.open("resources/example_3_1.txt");
    file.open("resources/input_3_1.txt");
    assert(file.is_open() && "Failed to open file");

    std::vector<std::string> schema;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) { continue; }
        schema.emplace_back(line);
    }

    i32 result = parse_part_numbers(schema);
    std::cout << result << std::endl;

    file.close();
}

}
