#include <iostream>
#include <func.h>
#include <windows.h>
#include <gtest/gtest.h>

std::string DESTINATION = "data/input.txt";

TEST(TestGroupName, Subtest_1) {
    ASSERT_TRUE(1 == 2);
}

TEST(TestGroupName, Subtest_2) {
    ASSERT_FALSE(2 == 2);
}


int main() {
//    ::testing::InitGoogleTest();
//    return RUN_ALL_TESTS();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    writeDistances(BreadthFirstSearch(1, DESTINATION));
    std::cout << std::endl;
    writeDots(findAllDots(DESTINATION));
}