#include <algorithm>
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
    usize begin_;
    usize end_;

public:
    Range(usize begin, usize end)
        : begin_(begin), end_(end)
    {}

    usize size() const
    {
        return end_ - begin_;
    }

    usize empty() const
    {
        return size() == 0;
    }

    std::optional<Range> intersect(Range rhs) const
    {
        if ((end_ < rhs.begin_) || (rhs.end_ < begin_)) {
            return std::nullopt;
        }

        return Range {
            std::max(begin_, rhs.begin_),
            std::min(end_, rhs.end_),
        };
    }

    std::optional<Range> intersect(usize val) const
    {
        return intersect({ val, val+1 });
    }

    Range limit_left(usize b) const
    {
        assert(b < end_);
        return Range {
            std::max(begin_, b),
            end_
        };
    }

    [[maybe_unused]] friend std::ostream&
    operator<<(std::ostream& os, Range r)
    {
        os << "Range: [" << r.begin_ << ", " << r.end_ << ")";
        return os;
    }
};

// ----------------------------------------------

class Conversion {
public:
    Range source_ = { 0, 0 };
    Range dest_ = { 0, 0 };

public:
    Conversion(std::string_view str)
    {
        usize pos = str.find(" ");

        std::from_chars(str.data(), str.data() + pos, dest_.begin_);
        str.remove_prefix(pos+1);

        pos = str.find(" ");

        std::from_chars(str.data(), str.data() + str.size(), source_.begin_);
        str.remove_prefix(pos+1);

        usize len = 0;
        std::from_chars(str.data(), str.data() + str.size(), len);

        dest_.end_ = dest_.begin_ + len;
        source_.end_ = source_.begin_ + len;
    }

    Conversion(Range r)
        : source_(r), dest_(r)
    {}

    [[maybe_unused]] friend std::ostream&
    operator<<(std::ostream& os, const Conversion& r)
    {
        os << "Map:" << r.source_ << " to "<< r.dest_;
        return os;
    }
};

// ----------------------------------------------

class Mapping {
public:
    std::string from_;
    std::string to_;
private:
    std::vector<Conversion> conversions_;

public:
    Mapping(std::string from, std::string to)
        : from_(from), to_(to)
    { }

    void add_map(Conversion conv)
    {
        auto it = conversions_.begin();
        while (it != conversions_.end()) {
            if (it->source_.begin_ > conv.source_.begin_) {
                break;
            }
            ++it;
        }

        conversions_.insert(it, conv);
    }

    void add_missing_maps()
    {
        usize b = 0;

        for (usize i = 0; i < conversions_.size(); ++i) {
            auto c = conversions_[i];
            if (c.source_.begin_ > b) {
                conversions_.insert((conversions_.begin() + i), Range {
                    b,
                    c.source_.begin_
                });
            }

            b = c.source_.end_;
        }

        if (conversions_.size() > 0) {
            conversions_.push_back(Range {
                (conversions_.end()-1)->source_.end_,
                std::numeric_limits<usize>::max(),
            });
        }
    }

    void map(std::vector<Range>& collector, Range r) const
    {
        bool did_inter = false;

        for (const auto& c : conversions_) {
            auto inter = c.source_.intersect(r);
            if (inter) {
                const usize start = c.dest_.begin_ + (inter->begin_ - c.source_.begin_);
                collector.push_back(Range {
                    start,
                    start + inter->size()
                });
                did_inter = true;
            } else if (did_inter) {
                break;
            }
        }
    }

    [[maybe_unused]] friend std::ostream&
    operator<<(std::ostream& os, const Mapping& m) {
        os << "Map [" << m.from_ << "] to [" << m.to_ << "]" << "\n";

        for (const auto& r : m.conversions_) {
            os << "\t" << r << "\n";
        }

        return os;
    }
};

// ----------------------------------------------

struct State {
    std::vector<usize> seeds;
    std::unordered_map<std::string, Mapping> from_maps;
    std::vector<Range> range_collector{};
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
            curr_map->add_missing_maps();
            state.from_maps.insert({curr_map->from_, *curr_map});
            curr_map = std::nullopt;
        }

        if (std::isdigit(line[0]) && curr_map) {
            curr_map->add_map(Conversion(line));
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
        curr_map->add_missing_maps();
        state.from_maps.insert({curr_map->from_, *curr_map});
        curr_map = std::nullopt;
    }

    return state;
}

usize find_closest(State& state, const std::string& from, Range range)
{
    if (from == "location") { return range.begin_; }

    auto it = state.from_maps.find(from);
    assert(it != state.from_maps.end());

    usize closest = std::numeric_limits<usize>::max();
    state.range_collector.clear();
    it->second.map(state.range_collector, range);

    for (const auto m : state.range_collector) {
        usize c = find_closest(state, it->second.to_, m);
        closest = std::min(closest, c);
    }

    return closest;
}

}

namespace aoc
{

// ----------------------------------------------

i64 solve_5_2(std::ifstream&& file)
{
    State state = parse_file(std::move(file));
    usize closest = std::numeric_limits<usize>::max();

    for (usize i = 0; i < state.seeds.size(); i+=2) {
        Range start { state.seeds[i], state.seeds[i] + state.seeds[i+1] };
        usize c = find_closest(state, "seed", start);
        closest = std::min(closest, c);
    }

    return closest;
}

// ----------------------------------------------

}
