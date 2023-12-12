#include "common.h"

#include <charconv>
#include <array>
#include <optional>
#include <tuple>


static std::string d3_input_test =
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

static auto valid_r_indexes = []<typename... Args>(std::vector<std::string_view>& data, Args ...args) {
    return std::make_tuple(std::clamp<int>(args, 0, data.size() - 1)...);
};

static auto valid_c_indexes = []<typename... Args>(std::vector<std::string_view>&data, Args ...args) {
    return std::make_tuple(std::clamp<int>(args, 0, data[0].size() - 1)...);
};

static auto get_number_if_valid = [](std::vector<std::string_view>& data, int r, int c_l, int c_r, bool b_chk_number) -> std::optional<int> {
    auto [start_c, end_c] = valid_c_indexes(data, c_l - 1, c_r + 1);
    auto [start_r, end_r] = valid_r_indexes(data, r   - 1, r   + 1);

    if (rng::any_of(data[start_r].substr(start_c, end_c - start_c + 1), is_symbol) ||
        rng::any_of(data[end_r  ].substr(start_c, end_c - start_c + 1), is_symbol) ||
        is_symbol(data[r][start_c]) || is_symbol(data[r][end_c]) ||
        !b_chk_number)
    {
        auto [start_num, end_num] = valid_c_indexes(data, c_l, c_r);
        std::string_view s = data[r].substr(start_num, end_num - start_num + 1);
        int num = 0;
        auto [_, res] = std::from_chars(s.data(), s.data() + s.size(), num);
        assert(res == std::errc());
        return num;
    }

    return std::nullopt;
};

static auto get_number_located_at = [](std::vector<std::string_view>& data, int r, int c, bool b_chk_number) -> std::optional<int> {
    if (!std::isdigit(data[r][c]))
        return std::nullopt;

    int c_l = c, c_r = c;
    while (c_l >= 0 && std::isdigit(data[r][c_l])) --c_l;
    while (c_r < data[r].size() && std::isdigit(data[r][c_r])) ++c_r;

    std::tie(c_l, c_r) = valid_c_indexes(data, c_l + 1, c_r - 1);
    return get_number_if_valid(data, r, c_l, c_r, b_chk_number);
};

TEST(AdventOfCode_2023, Day3_1_ranges) {
    int sum = 0;
    std::string input;
    read_input(input);
    //input = d3_input_test;

    std::vector<std::string_view> data;
    for (auto&& line : input | views::split('\n'))
        data.push_back(line);

    int i = 0;
    for (int r : rng::iota_view{ 0ull, data.size() }) // C++23: views::enumerate(data)
    {
        auto line = data[r];
        for (int c = 0; c < line.size(); ++c)
        {
            auto it_beg = std::find_if(line.begin() + c, line.end(), is_digit);
            if (it_beg == line.end())
                break;

            if (auto num = get_number_located_at(data, r, rng::distance(line.begin(), it_beg), true); num)
                sum += num.value();

            c = std::distance(line.begin(), std::find_if_not(it_beg, line.end(), is_digit));
        }
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 539'713);
}

TEST(AdventOfCode_2023, Day3_1_legacy) {
    int sum = 0;
    std::string input;
    read_input(input);
    //input = d3_input_test;

    std::vector<std::string_view> data;
    for (auto&& line : input | views::split('\n'))
        data.push_back(line);

    for (int r : rng::iota_view{ 0ull, data.size() })
    {
        auto line = data[r];
        for (int c = 0; c < line.size(); ++c)
        {
            auto it_beg = std::find_if(line.begin() + c, line.end(), is_digit);
            if (it_beg == line.end())
                break;

            if (std::isdigit(line[c])) {
                if (auto num = get_number_located_at(data, r, c, true); num)
                    sum += num.value();

                c = std::distance(line.begin(), std::find_if_not(it_beg, line.end(), is_digit));
            }

        }
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 539'713);
}

TEST(AdventOfCode_2023, Day3_2_legacy) {
    int sum = 0;
    std::string input;
    read_input(input);
    //input = d3_input_test;

    std::vector<std::string_view> data;
    for (auto&& line : input | views::split('\n'))
        data.push_back(line);

    for (int r : rng::iota_view{ 0ull, data.size() })
    {
        for (int c = 0; c < data[r].size(); ++c)
        {
            if (data[r][c] != '*')
                continue;
            
            auto [start_r, end_r] = valid_r_indexes(data, r - 1, r + 1);
            auto [start_c, end_c] = valid_c_indexes(data, c - 1, c + 1);

            int num_numbers = 0;
            int num_mul = 1;
            for (int r : rng::iota_view{ start_r, end_r + 1 })
            {
                bool b_was_num = false;
                for (int c : rng::iota_view{ start_c, end_c + 1 })
                {
                    if (!std::isdigit(data[r][c])) {
                        b_was_num = false;
                        continue;
                    }

                    if (b_was_num)
                        continue;

                    b_was_num = true;
                    if (auto num = get_number_located_at(data, r, c, true); num) {
                        num_numbers++;
                        num_mul *= num.value();
                    }
                }
            }

            if (num_numbers == 2)
                sum += num_mul;
        }
    }

    GTEST_COUT << "answer is " << sum << std::endl;
    EXPECT_EQ(sum, 84'159'075);
}
