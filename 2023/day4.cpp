#include "common.h"

#include <charconv>
#include <array>
#include <optional>
#include <tuple>
#include <regex>


using regex_it = std::sregex_iterator;

static std::string d4_input_test =
"Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53\n"
"Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19\n"
"Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1\n"
"Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83\n"
"Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36\n"
"Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11";

static auto str_to_int = [](std::string_view s) {
    int num = 0;
    auto [_, res] = std::from_chars(s.data(), s.data() + s.size(), num);
    assert(res == std::errc());
    return num;
};


TEST(AdventOfCode_2023, Day4_1) {
    int sum = 0;
    std::string input;
    read_input(input);
    //input = d4_input_test;

    int i = 0;
    for (auto&& line :  input |
                        views::split('\n') |
                        views::transform([](auto&& r) { return r | views::drop_while([](char ch) { return ch != ':'; }) | views::drop(2); }))
    {
        auto it_sep = rng::find_first_of(line, "|");
        rng::subrange win_rng(line.begin(), it_sep), my_rng(it_sep, line.end());

        std::set<int> win_nums, player_nums;
        std::regex num_regex("(\\d+)");
        rng::for_each(
            rng::subrange(regex_it{ win_rng.begin(), win_rng.end(), num_regex }, regex_it{}),
            [&win_nums](auto&& match) { win_nums.insert(str_to_int(match.str())); });

        rng::for_each(
            rng::subrange(regex_it{ my_rng.begin(), my_rng.end(), num_regex }, regex_it{}),
            [&player_nums](auto&& match) { player_nums.insert(str_to_int(match.str())); });

        std::vector<int> v;
        rng::set_intersection(win_nums, player_nums, std::back_inserter(v));
        if (!v.empty())
            sum += std::pow(2, v.size() - 1);
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 22'674);
}
