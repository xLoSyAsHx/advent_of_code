#include "common.h"

#include <charconv>
#include <array>


std::string d2_1_input_test =
": 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green \n"
": 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue \n"
": 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red \n"
": 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red \n"
": 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green ";


TEST(AdventOfCode_2023, Day2_1) {
    int sum = 0, id = 0;

    std::string input;
    read_input(input);

    auto [e_beg, e_end] = rng::remove_if(input, [](auto c) { return c == ',' || c == ';'; });
    input.erase(e_beg, e_end);

    for (std::string_view line : input | views::split('\n'))
    {
        line = line.substr(line.find_first_of(':') + 2);

        ++id;
        bool b_game_possible = true;
        while (!line.empty())
        {
            auto sep_offset = line.find_first_of(' ');
            auto left  = line.substr(0, sep_offset);
            line = line.substr(sep_offset + 1);

            sep_offset = line.find_first_of(' ');
            auto right = line.substr(0, sep_offset);
            line = line.substr(sep_offset + 1);

            int n_dice = 0;
            auto [_, res] = std::from_chars(left.data(), left.data() + left.size(), n_dice);
            ASSERT_EQ(res, std::errc());

            if (right == "red"   && n_dice > 12 ||
                right == "green" && n_dice > 13 ||
                right == "blue"  && n_dice > 14) {
                b_game_possible = false;
                break;
            }

            if (sep_offset == std::string_view::npos)
                break;
        }
        if (b_game_possible)
            sum += id;
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 2'268);
}

TEST(AdventOfCode_2023, Day2_2) {
    int sum = 0, id = 0;

    std::string input;
    read_input(input);

    auto [e_beg, e_end] = rng::remove_if(input, [](auto c) { return c == ',' || c == ';'; });
    input.erase(e_beg, e_end);

    int r{}, g{}, b{};
    for (std::string_view line : input | views::split('\n'))
    {
         r = g = b = 0;
        line = line.substr(line.find_first_of(':') + 2);

        ++id;
        while (!line.empty())
        {
            auto sep_offset = line.find_first_of(' ');
            auto left = line.substr(0, sep_offset);
            line = line.substr(sep_offset + 1);

            sep_offset = line.find_first_of(' ');
            auto color = line.substr(0, sep_offset);
            line = line.substr(sep_offset + 1);

            int n_dice = 0;
            auto [_, res] = std::from_chars(left.data(), left.data() + left.size(), n_dice);
            ASSERT_EQ(res, std::errc());

            if      (color[0] =='r') r = std::max(r, n_dice);
            else if (color[0] =='g') g = std::max(g, n_dice);
            else if (color[0] =='b') b = std::max(b, n_dice);
            
            if (sep_offset == std::string_view::npos)
                break;
        }
        sum += r * g * b;
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 63'542);
}