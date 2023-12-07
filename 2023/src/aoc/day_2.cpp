#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <hell/common.h>
#include <unordered_map>


namespace aoc
{

struct Limit {
    std::string_view kind;
    usize num;
};

static void parse_combination(std::unordered_map<std::string_view, usize>& maxes,
                             std::string_view combi)
{
    std::string_view token = combi.substr(0, combi.find(" "));
    usize number = std::stoi(token.data());
    combi.remove_prefix(token.size() + 1);

    // NOTE(lm): maxes is guaranteed to be 0 initialized
    maxes[combi] = std::max(maxes[combi], number);
}

static void round_is_valid(std::unordered_map<std::string_view, usize>& maxes,
                           std::string_view round)
{
    usize pos = 0;
    std::string_view token;

    while ((pos = round.find(",")) != std::string_view::npos) {
        // NOTE(lm): 1 to skip first space
        token = round.substr(1, pos - 1);
        round.remove_prefix(token.size() + 2);

        parse_combination(maxes, token);
    }

    token = round.substr(1, pos - 1);
    parse_combination(maxes, token);
}

static bool round_within_limits(const Limit limits[], usize size,
                                const std::unordered_map<std::string_view, usize>& maxes)
{
    for (usize i = 0; i < size; ++i) {
        if (maxes.at(limits[i].kind) > limits[i].num) {
            return false;
        }
    }

    return true;
}

void solve_day_2()
{
    const Limit limits[3] = {
        { "red", 12 },
        { "green", 13 },
        { "blue", 14 },
    };
    std::unordered_map<std::string_view, usize> maxes {};

    std::ifstream file;
    file.open("resources/input_2_1.txt");
    assert(file.is_open() && "Failed to open file");

    std::string buf;
    usize limit_result = 0;
    usize maxes_result = 0;

    while (std::getline(file, buf)) {
        if (buf.empty()) { continue; }

        std::string_view line { buf };
        std::string_view token;
        usize pos = 0;
        usize id;

        // reset
        {
            maxes["red"] = 0;
            maxes["blue"] = 0;
            maxes["green"] = 0;
        }

        // parse id
        {
            pos = line.find(":");

            token = line.substr(5, pos - 5);
            id = std::stoi(token.data());

            line.remove_prefix(pos + 1);
        }

        // parse game rounds
        {
            while ((pos = line.find(";")) != std::string_view::npos) {
                token = line.substr(0, pos);
                round_is_valid(maxes, token);
                line.remove_prefix(token.size() + 1);
            }
            round_is_valid(maxes, line);

            usize tmp = 1;
            for (const auto& [kind, value]: maxes) {
                tmp *= value;
            }
            maxes_result += tmp;

        }

        // check limits
        {
            if (round_within_limits(limits, ARRAY_SIZE(limits), maxes)) {
                limit_result += id;
            }
        }
    }

    file.close();

    std::cout << "Limits: " << limit_result << "\n"
              << "Maxes: " << maxes_result << std::endl;
}

}
