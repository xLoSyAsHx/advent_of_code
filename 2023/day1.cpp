#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <string_view>
#include <ranges>
#include <format>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <functional>

#include <gtest/gtest.h>

#include "day1_input.h"

std::string input_test =
"two1nine\n"
"eightwothree\n"
"abcone2threexyz\n"
"xtwone3four\n"
"4nineeightseven2\n"
"zoneight234\n"
"7pqrstsixteen";

namespace ranges = std::ranges;
namespace views = std::ranges::views;
#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

TEST(AdventOfCode_2023, Day1_1) {
    int sum = 0;
    auto isdigit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
    for (std::string_view s : views::split(day1_input, '\n')) {
        //std::cout << std::format("s = {}\n", s);
        int v1 = *std::find_if(s.begin(), s.end(), isdigit) - '0';
        int v2 = *std::find_if(s.rbegin(), s.rend(), isdigit) - '0';
        sum += v1 * 10 + v2;
    }

    GTEST_COUT << "Day1.1 answer " << sum << std::endl;
    EXPECT_EQ(sum, 53'921);
}

TEST(AdventOfCode_2023, Day1_2) {
    int sum = 0;
    std::array<std::string, 9> substrings = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    // Starting from C++23: std::unordered_set<char> unique_chars = substrings | views::transform([](auto&& r) { return r[0]; }) | ranges::to<std::unordered_set<char>>;
    std::unordered_set<char> first_subs_ch;
    ranges::for_each(
        substrings | views::transform([](auto&& r) { return r[0]; }),
        [&first_subs_ch](char c){ first_subs_ch.insert(c); });

    for (auto&& s : views::split(day1_input, '\n'))
    {
        for (int i = 0; i < ranges::size(s); ++i) {
            if (!first_subs_ch.contains(s[i]))
                continue;

            auto&& s_view = s | views::drop(i);
            for (int j = 0; j < substrings.size(); ++j) {
                if (substrings[j] == std::string_view(s_view | views::take(substrings[j].length()))) {
                    s_view.front() = '0' + j + 1;
                    break;
                }
            }
        }

        int v1 = *std::find_if(s.begin(), s.end(), isdigit) - '0';

        auto r_s = views::reverse(s);
        int v2 = *std::find_if(r_s.begin(), r_s.end(), isdigit) - '0';
        sum += v1 * 10 + v2;
        //std::cout << std::format("v1 = {}  v2 = {}\n", v1, v2);
    }

    GTEST_COUT << "Day1.2 answer " << sum << std::endl;
    EXPECT_EQ(sum, 54676);
}
