#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <hell/common.h>
#include <unordered_map>


namespace aoc
{

struct Condition {
    std::string_view kind;
    usize num;
};

static std::unordered_map<std::string_view, usize>
create_conditions(const Condition* conditions, usize size)
{
    std::unordered_map<std::string_view, usize> result {};

    for (usize i = 0; i < size; ++i) {
        result[conditions[i].kind] = conditions[i].num;
    }

    return result;
}

static bool combination_is_valid(const std::unordered_map<std::string_view, usize>& cond,
                                 std::string_view combi)
{
    std::string_view token = combi.substr(0, combi.find(" "));
    usize number = std::stoi(token.data());
    combi.remove_prefix(token.size() + 1);

    const auto it = cond.find(combi);
    if (it != cond.end()) {
        if (number > it->second) {
            return false;
        }
    }

    return true;
}

static bool round_is_valid(const std::unordered_map<std::string_view, usize>& cond,
                           std::string_view round)
{
    usize pos = 0;
    std::string_view token;

    while ((pos = round.find(",")) != std::string_view::npos) {
        // NOTE(lm): 1 to skip first space
        token = round.substr(1, pos - 1);
        round.remove_prefix(token.size() + 2);

        if (!combination_is_valid(cond, token)) {
            return false;
        }
    }

    token = round.substr(1, pos - 1);
    if (!combination_is_valid(cond, token)) {
        return false;
    }

    return true;
}

i64 solve_2_1(std::ifstream&& file)
{
    const Condition c[3] = {
        { "red", 12 },
        { "green", 13 },
        { "blue", 14 },
    };
    const auto cond = create_conditions(c, ARRAY_SIZE(c));

    std::string buf;
    usize result = 0;

    while (std::getline(file, buf)) {
        if (buf.empty()) { continue; }

        std::string_view line { buf };
        std::string_view token;
        usize pos = 0;
        usize id;

        // parse id
        {
            pos = line.find(":");

            token = line.substr(5, pos - 5);
            id = std::stoi(token.data());

            line.remove_prefix(pos + 1);
        }

        // parse game rounds
        {
            bool valid = true;
            while (valid && (pos = line.find(";")) != std::string_view::npos) {
                token = line.substr(0, pos);
                valid &= round_is_valid(cond, token);
                line.remove_prefix(token.size() + 1);
            }
            valid &= round_is_valid(cond, line);

            if (valid) {
                result += id;
            }
        }
    }

    return result;
}

}

