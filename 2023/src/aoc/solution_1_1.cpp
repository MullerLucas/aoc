#include <cassert>
#include <cctype>
#include <fstream>
#include <string>
#include <iostream>

#include <hell/common.h>

namespace aoc
{

// ----------------------------------------------

void solve_1_1()
{
    std::ifstream file;
    file.open("resources/input_1_1.txt");
    assert(file.is_open() && "Failed to open file");

    std::string line;
    i32 sum = 0;

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

        sum += ((line[left] - '0') * 10) + (line[right] - '0');
    }

    std::cout << "Sum: " << sum << std::endl;

    file.close();
}

// ----------------------------------------------

}
