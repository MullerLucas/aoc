#include <cassert>
#include <cctype>
#include <cmath>
#include <fstream>
#include <hell/common.h>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::vector<i32> parse_numbers(std::string_view str)
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
public:
    usize quantity_;
private:
    std::vector<i32> win_numbers_;
    std::vector<i32> my_numbers_;

public:
    Card(std::string_view str)
        : quantity_(1)
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

    u32 count_winners() const
    {
        usize winners = 0;

        for (const auto n : my_numbers_) {
            if (is_winner(n)) {
                ++winners;
            }
        }

        return winners;
    }

    u32 calc_points() const
    {
        const usize winners = count_winners();

        if (winners == 0) {
            return 0;
        }
        return std::pow(2, winners-1);
    }
};

}

namespace aoc
{

i64 solve_4_2(std::ifstream&& file)
{
    std::string line;
    std::vector<Card> cards{};

    while (std::getline(file, line)) {
        cards.emplace_back(line);
    }

    i64 result = 0;

    for (usize i = 0; i < cards.size(); ++i) {
        const usize quantity = cards[i].quantity_;
        usize winners = cards[i].count_winners();

        result += quantity;

        usize c = i + 1;
        while (winners > 0) {
            if (c < cards.size()) {
                cards[c].quantity_ += quantity;
            }
            ++c;
            --winners;
        }
    }

    return result;
}

}
