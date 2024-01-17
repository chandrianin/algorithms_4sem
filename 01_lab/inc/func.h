#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>

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
                } else if (inCoordinateFlag && currentInput[i] != ' ') {
                    throw std::exception("Неизвестный символ");
                }
                break;
        }
    }
}

//  Возвращает true, если C находится левее вектора AB, и false в противном случае
bool leftOrRight(std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C) {

    // Возвращает значение функции-вектора AB в точке x
    auto func{[&B, &A](double x) { return (B.second - A.second) * (x - A.first) / (B.first - A.first) + A.second; }};
    // true, если вектор AB направлен слева направо, false в противном случае
    bool funcDirection = B.first > A.first;

    // true, если точка C находится выше вектора AB, false в противном случае
    bool verticalPosition = C.second > func(C.first);

    return funcDirection == verticalPosition;
}

std::vector<std::pair<double, double>> grahamScan(const std::vector<std::pair<double, double>> &inputCoordinates) {
    std::vector<unsigned int> indices{};
    for (int i = 0; i < inputCoordinates.size(); i++) {
        indices.push_back(i);
    }
    for (int i = 0; i < inputCoordinates.size(); i++) {
        if (inputCoordinates[indices[i]].first < inputCoordinates[indices[0]].first) {
            std::swap(indices[0], indices[i]);
        }
    }

    if (inputCoordinates.size() <= 2) {
        return std::vector<std::pair<double, double >>{inputCoordinates[indices[0]], inputCoordinates[indices[1]]};
    }

    for (int i = 2; i < inputCoordinates.size(); i++) {
        int j = i;
        while (j > 1 && !leftOrRight(inputCoordinates[indices[0]],
                                     inputCoordinates[indices[j - 1]],
                                     inputCoordinates[indices[j]])) {
            std::swap(indices[j - 1], indices[j]);
            j -= 1;
        }
    }

    std::vector<unsigned int> resultIndices{indices[0], indices[1]};
    for (int i = 2; i < indices.size(); i++) {
        while (!leftOrRight(inputCoordinates[resultIndices[resultIndices.size() - 2]], inputCoordinates[resultIndices[resultIndices.size() - 1]], inputCoordinates[indices[i]])) {
            resultIndices.pop_back();
        }
        resultIndices.push_back(indices[i]);
    }

    std::vector<std::pair<double, double>> resultCoordinates{};
    for (auto i : resultIndices) {
        resultCoordinates.push_back(inputCoordinates[i]);
    }
    return resultCoordinates;
}