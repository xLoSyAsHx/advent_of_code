#include "common.h"

#include <charconv>
#include <array>


static std::string d3_input_test =
"123#321...\n"
"...123#321\n"
"..........\n"
"467..114..\n"
"...*......\n"
"..35..633.\n"
"......#...\n"
"617*......\n"
"..... + .58.\n"
"..592.....\n"
"......755.\n"
"...$.*....\n"
".664.598..";

static auto is_symbol = [](char c) { return !std::isdigit(c) && c != '.'; };
static auto is_digit = [](char c) { return std::isdigit(c); };

TEST(AdventOfCode_2023, Day3_1) {
    int sum = 0, id = 0;

    std::string input;
    read_input(input);
    //input = d3_input_test;

    std::vector<std::string_view> data;
    for (auto&& line : input | views::split('\n'))
        data.push_back(line);

    int i = 0;
    int max_i = data.size() - 1;
    int line_size = data[0].size();
    for (auto&& line : input | views::split('\n'))
    {
        for (int j = 0; j < line.size(); ++j)
        {
            auto tmp = line.next(j);
            auto it_beg = rng::find_if(tmp, is_digit);
            if (it_beg == line.end())
                break;

            auto it_end = std::find_if_not(it_beg, line.end(), is_digit);

            int start = std::clamp<int>(std::distance(line.begin(), it_beg) - 1, 0, line_size);
            int end   = std::distance(line.begin(), it_end) + 1;

            auto l1 = data[std::clamp(i - 1, 0, max_i)].substr(start, end - start);
            auto l2 = data[std::clamp(i + 1, 0, max_i)].substr(start, end - start);

            if (rng::any_of(l1, is_symbol) || rng::any_of(l2, is_symbol) ||
                is_symbol(line[start]) || is_symbol(line[std::min(end, line_size) - 1]))
            {
                auto number_rng = rng::subrange(it_beg, it_end);
                int num = 0;
                auto [_, res] = std::from_chars(number_rng.data(), number_rng.data() + number_rng.size(), num);
                ASSERT_EQ(res, std::errc());
                sum += num;
                std::cout << std::format("s = {}\n", num);
            }
            j = end - 1;
        }
        i++;
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 539'713);
}
