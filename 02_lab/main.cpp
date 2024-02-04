#include <iostream>
#include <func.h>
#include <windows.h>

std::string DESTINATION = "data/input.txt";

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    writeDistances(BreadthFirstSearch(4, DESTINATION));
    std::cout << std::endl;
    writeDots(findAllDots(DESTINATION));
}