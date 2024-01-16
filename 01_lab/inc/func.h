#pragma once

#include <iostream>
#include <vector>
#include <string>

void coordinatesInput(std::vector<std::pair<double, double>> &inputVector) {
    std::cout << "Вводите поочередно, разделяя запятыми, координаты точек следующего вида:" << std::endl;
    std::cout << "(1;5)" << std::endl;
    std::string currentInput;
    std::getline(std::cin, currentInput);
    bool inCoordinateFlag = false;
    std::string currentNumber{};
    std::pair<double, double> currentPair{};
    for (int i = 0; i < currentInput.length(); i++) {
        switch (currentInput[i]) {
            case '(':
                if (!inCoordinateFlag) { inCoordinateFlag = true; }
                else { throw std::exception("Некорректно введены символы"); }
                break;
            case ';':
                if (inCoordinateFlag) {
                    // записать первое число в пару
                    currentPair.first = stod(currentNumber);
                    currentNumber = "";
                } else { throw std::exception("Некорректно введены символы"); }

                break;
            case ')':
                if (inCoordinateFlag) {
                    // записать второе число в пару и добавить пару в вектор
                    currentPair.second = stod(currentNumber);
                    currentNumber = "";
                    inputVector.push_back(currentPair);
                    inCoordinateFlag = false;
                } else { throw std::exception("Некорректно введены символы"); }
                break;
            default:
                if (inCoordinateFlag && (currentInput[i] == '.' || isdigit(currentInput[i]))) {
                    currentNumber += currentInput[i];
                } else if (inCoordinateFlag) {
                    throw std::exception("Неизвестный символ");
                }
                break;
        }
    }
}