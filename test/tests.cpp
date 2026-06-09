// Copyright 2020 GHA
#include <gtest/gtest.h>

#include <vector>

#include "tree.h"

TEST(ads9, test1) {
    PMTree tree(std::vector<char>{'1', '2', '3'});
    std::vector<char> result = getPerm1(tree, 1);
    ASSERT_TRUE(result[0] == '1' && result[1] == '2' && result[2] == '3');
}

TEST(ads9, test2) {
    PMTree tree(std::vector<char>{'1', '2', '3'});
    std::vector<char> result = getPerm2(tree, 2);
    ASSERT_TRUE(result[0] == '1' && result[1] == '3' && result[2] == '2');
}

TEST(ads9, test3) {
    PMTree tree(std::vector<char>{'1', '2', '3'});
    std::vector<char> result = getPerm1(tree, 6);
    ASSERT_TRUE(result[0] == '3' && result[1] == '2' && result[2] == '1');
}

TEST(ads9, test4) {
    PMTree tree(std::vector<char>{'1', '2', '3'});
    std::vector<char> result = getPerm2(tree, 8);
    ASSERT_EQ(result.size(), 0);
}
