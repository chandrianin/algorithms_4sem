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

//(0;0), (0; 100), (100;0), (5; 0), (3; 0), (8; 0), (0; 13), (50; 50), (35; 20)