#include <cassert>
#include <cctype>
#include <cmath>
#include <fstream>
#include <hell/common.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace aoc
{

static std::vector<i32> parse_numbers(std::string_view str)
{
    std::vector<i32> result;

    bool isset = false;
    i32 tmp = 0;
    for (const char c : str) {
        if (std::isdigit(c)) {
            tmp = tmp * 10 + (c - '0');
            isset = true;
        } else if (isset) {
            result.push_back(tmp);
            tmp = 0;
            isset = false;
        }
    }

    if (isset) {
        result.push_back(tmp);
    }

    return result;
}

class Card {
private:
    std::vector<i32> win_numbers_;
    std::vector<i32> my_numbers_;

public:
    Card(std::string_view str)
    {
        const usize end_p1 = str.find(":");
        const usize end_p2 = str.find("|");

        win_numbers_ = parse_numbers(str.substr(end_p1 + 1, end_p2 - end_p1 - 1));
        my_numbers_  = parse_numbers(str.substr(end_p2 + 1, str.size() - end_p2 - 1));
    }

    bool is_winner(i32 num) const
    {
        for (const auto n : win_numbers_) {
            if (n == num) {
                return true;
            }
        }

        return false;
    }

    u32 calc_points() const
    {
        usize matches = 0;

        for (const auto n : my_numbers_) {
            if (is_winner(n)) {
                ++matches;
            }
        }

        if (matches == 0) {
            return 0;
        }
        return std::pow(2, matches-1);
    }

};

i64 solve_4_1(std::ifstream&& file)
{
    std::string line;

    i64 result = 0;

    while (std::getline(file, line)) {
        const Card card{line};
        result += card.calc_points();
    }

    return result;
}

}
