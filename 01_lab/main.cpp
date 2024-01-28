#include <iostream>
#include <vector>
#include <func.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    std::vector<std::pair<double, double>> coordinates{};
    try {
        coordinatesInput(coordinates);

    }
    catch (std::exception &error) {
        std::cerr << error.what() << std::endl;
    }
    auto outVector = grahamScan(coordinates);
    for (auto dot: outVector) {
        std::cout << "(" << dot.first << ";" << dot.second << ")" << std::endl;
    }
}