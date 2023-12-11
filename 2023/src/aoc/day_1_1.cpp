#include <cassert>
#include <cctype>
#include <fstream>
#include <string>
#include <iostream>

#include <hell/common.h>

// ----------------------------------------------

namespace aoc
{

i64 solve_1_1(std::ifstream&& file)
{
    std::string line;
    i64 result = 0;

    while (std::getline(file, line)) {
        if (line.empty()) { continue; }

        usize left = 0;
        while (!isdigit(line[left])) {
            ++left;
        }

        usize right = line.size()-1;
        while (!isdigit(line[right])) {
            --right;
        }

        result += ((line[left] - '0') * 10) + (line[right] - '0');
    }

    return result;
}

}

// ----------------------------------------------
