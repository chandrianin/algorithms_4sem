#include <iostream>
#include <func.h>
#include <windows.h>
#include <gtest/gtest.h>
#include <googleTestFunc.h>

std::string DESTINATION = "data/input.txt";

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ::testing::InitGoogleTest();
    ::testing::GTEST_FLAG(catch_exceptions) = false;
    return RUN_ALL_TESTS();
    writeDistances(depthFirstSearch(4, DESTINATION));
    std::cout << std::endl;
    writeDots(findAllDots(DESTINATION));
}