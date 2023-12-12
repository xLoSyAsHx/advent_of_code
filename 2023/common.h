#pragma once

#include <gtest/gtest.h>

#include <fstream>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <string>


namespace rng = std::ranges;
namespace views = std::ranges::views;
#define GTEST_COUT std::cerr << "[          ] "
#define GTEST_TEST_NAME ::testing::UnitTest::GetInstance()->current_test_info()->name()

static void read_input(std::string& s)
{
    std::string test_name = GTEST_TEST_NAME;
    test_name = test_name.substr(0, test_name.find_first_of('_'));

    std::ifstream f(DATA_FILES_PATH + test_name + "_input.txt", std::ios::ate);
    EXPECT_TRUE(f.is_open());

    auto size = f.tellg();
    f.seekg(0);
    s.resize(size);
    f.read(&s[0], size);
}
