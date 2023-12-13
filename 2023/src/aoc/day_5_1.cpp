#include <cassert>
#include <cctype>
#include <charconv>
#include <fstream>
#include <hell/common.h>
#include <iostream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace
{

// ----------------------------------------------

class Range {
public:
    usize source_start_;
    usize dest_start_;
    usize len_;

public:
    Range(std::string_view str)
    {
        usize pos = str.find(" ");

        std::from_chars(str.data(), str.data() + pos, dest_start_);
        str.remove_prefix(pos+1);

        pos = str.find(" ");

        std::from_chars(str.data(), str.data() + str.size(), source_start_);
        str.remove_prefix(pos+1);

        std::from_chars(str.data(), str.data() + str.size(), len_);
    }

    [[maybe_unused]] friend std::ostream&
    operator<<(std::ostream& os, const Range& r)
    {
        os << "Range - D:" << r.dest_start_ << " S:" << r.source_start_
            << " L:" << r.len_;
        return os;
    }
};

// ----------------------------------------------

class Mapping {
public:
    std::string from_;
    std::string to_;
private:
    std::vector<Range> ranges_;

public:
    Mapping(std::string from, std::string to)
        : from_(from), to_(to)
    { }

    void add_range(Range range)
    {
        auto it = ranges_.begin();
        while (it != ranges_.end() && it->source_start_ < range.source_start_) {
            ++it;
        }
        ranges_.insert(it, range);
    }

    usize map(usize source)
    {
        for (const auto& r : ranges_) {
            if ((r.source_start_ <= source) && (r.source_start_+r.len_ >= source)) {
                return r.dest_start_ + (source - r.source_start_);
            }
        }

        return source;
    }

    [[maybe_unused]] friend std::ostream&
    operator<<(std::ostream& os, const Mapping& m) {
        os << "Map [" << m.from_ << "] to [" << m.to_ << "]" << "\n";

        for (const auto& r : m.ranges_) {
            os << "\t" << r << "\n";
        }

        return os;
    }
};

// ----------------------------------------------

struct State {
    std::vector<usize> seeds;
    std::unordered_map<std::string, Mapping> from_maps;
};

// ----------------------------------------------

std::vector<usize> parse_seeds(std::string_view str)
{
    std::vector<usize> result;

    str.remove_prefix(7);

    usize pos;
    usize val;
    while ((pos = str.find(" ")) != str.npos) {
        std::from_chars(str.data(), str.data() + pos, val);
        result.push_back(val);
        str.remove_prefix(pos + 1);
    }

    std::from_chars(str.data(), str.data() + str.size(), val);
    result.push_back(val);

    return result;
}

std::pair<std::string, std::string> parse_map_name(std::string_view str)
{
    usize pos = str.find("-");
    std::string from { str.substr(0, pos) };

    str.remove_prefix(pos+4);

    pos = str.find(" ");
    std::string to { str.substr(0, pos) };

    return { from, to };
}

// ----------------------------------------------

State parse_file(std::ifstream&& file)
{
    State state{};
    std::string line;
    std::optional<Mapping> curr_map = std::nullopt;

    while (std::getline(file, line)) {
        if (line.empty() && curr_map) {
            state.from_maps.insert({curr_map->from_, *curr_map});
            curr_map = std::nullopt;
        }

        if (std::isdigit(line[0]) && curr_map) {
            curr_map->add_range(Range(line));
        } else {
            usize pos = line.find("seeds: ");
            if (pos != line.npos) {
                state.seeds = parse_seeds(line);
            } else {
                const auto names = parse_map_name(line);
                curr_map = Mapping { names.first, names.second };
            }
        }
    }

    if (curr_map) {
        state.from_maps.insert({curr_map->from_, *curr_map});
        curr_map = std::nullopt;
    }

    return state;
}

}

namespace aoc
{

// ----------------------------------------------

i64 solve_5_1(std::ifstream&& file)
{
    State state = parse_file(std::move(file));
    usize closets = std::numeric_limits<usize>::max();

    for (const auto seed : state.seeds) {
        std::string_view from = "seed";
        usize val = seed;

        while (from != "location") {
            const auto map = state.from_maps.find(std::string(from));
            assert(map != state.from_maps.end());

            val = map->second.map(val);
            from = map->second.to_;
        }

        closets = std::min(closets, val);
    }

    return closets;
}

// ----------------------------------------------

}
