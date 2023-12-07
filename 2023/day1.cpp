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

#include "day1_input.h"

std::string input_test =
"two1nine\n"
"eightwothree\n"
"abcone2threexyz\n"
"xtwone3four\n"
"4nineeightseven2\n"
"zoneight234\n"
"7pqrstsixteen";

namespace views = std::ranges::views;

int main() {
    int sum = 0;
    auto isdigit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
    for (std::string_view s : views::split(day1_input, '\n')) {
        //std::cout << std::format("s = {}\n", s);
        int v1 = *std::find_if(s.begin(), s.end(), isdigit) - '0';
        int v2 = *std::find_if(s.rbegin(), s.rend(), isdigit) - '0';
        sum += v1 * 10 + v2;
    }
    std::cout << "\nDay1.1 answer " << sum;

    sum = 0;
    std::array<std::string, 9> substings = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    std::unordered_set<char> first_subs_ch = {
        'o', 't', 't', 'f', 'f', 's', 's', 'e', 'n'
    };

    auto v1 = views::split(day1_input, '\n');
    auto vv = v1.base()[0];

    for (auto&& s : views::split(day1_input, '\n'))
    {
        for (int i = 0; i < std::ranges::size(s); ++i) {
            if (!first_subs_ch.contains(s[i]))
                continue;

            auto&& s_view = s | views::drop(i);
            for (int j = 0; j < substings.size(); ++j) {
                if (substings[j] == std::string_view(s_view | views::take(substings[j].length()))) {
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
    std::cout << "\nDay1.2 answer " << sum;

    return 0;
}