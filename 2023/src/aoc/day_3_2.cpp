#include <cassert>
#include <cctype>
#include <fstream>
#include <string>
#include <iostream>
#include <hell/common.h>
#include <vector>
#include <functional>



namespace aoc
{

static void find_connected(std::vector<std::tuple<usize, usize>>& collector,
                           const std::vector<std::string>& schema,
                           usize x_start, usize y_start, usize len,
                           std::function<bool(char c)> pred)
{
    i64 x_left  = static_cast<i64>(x_start) - 1;
    i64 x_right = static_cast<i64>(x_start) + len;

    i64 y_to_check[] {
        static_cast<i64>(y_start) - 1,
        static_cast<i64>(y_start) + 1,
    };

    const auto check = [&schema, pred](i64 x, i64 y) -> bool {
        return (y >= 0 && y < static_cast<i64>(schema.size())) &&
               (x >= 0 && x < static_cast<i64>(schema.at(y).size())) &&
               pred(schema.at(y).at(x));
    };

    // check below and above
    for (auto y : y_to_check) {
        bool contg = false;

        for (i64 x = x_left; x <= x_right; ++x) {
            if (check(x, y)) {
                if (!contg) {
                    contg = true;
                    collector.emplace_back(x, y);
                }
            } else {
                contg = false;
            }
        }
    }

    if (check(x_left, y_start)) {
        collector.emplace_back(x_left, y_start);
    }

    if(check(x_right, y_start)) {
        collector.emplace_back(x_right, y_start);
    }
}

static u64 parse_number(const std::vector<std::string>& schema, usize x, usize y)
{
    // find the first digit
    while (x > 0 && std::isdigit(schema.at(y).at(x-1))) {
        --x;
    }

    u64 result = 0;
    while ((x < schema.at(y).size()) && std::isdigit(schema.at(y).at(x))) {
        const auto d = static_cast<u64>(schema.at(y).at(x)) - 48;
        result = result * 10 + d;
        ++x;
    }

    return result;
}

static u64 parse_gear_numbers(const std::vector<std::string>& schema)
{
    const auto pred = [](char c) -> bool {
        return isdigit(c);
    };

    u64 result = 0;
    std::vector<std::tuple<usize, usize>> collector {};
    collector.reserve(2);

    for (usize y = 0; y < schema.size(); ++y) {
        for (usize x = 0; x < schema.at(y).size(); ++x) {
            if (schema.at(y).at(x) != '*') { continue; }

            collector.clear();
            find_connected(collector, schema, x, y, 1, pred);
            if (collector.size() != 2) { continue; }

            u64 tmp = 1;
            for (const auto& coord: collector) {
                tmp *= parse_number(schema, std::get<0>(coord), std::get<1>(coord));
            }
            result += tmp;
        }
    }

    return result;
}

i64 solve_3_2(std::ifstream&& file)
{
    std::vector<std::string> schema;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) { continue; }
        schema.emplace_back(line);
    }

    i64 result = parse_gear_numbers(schema);
    return result;
}

}
