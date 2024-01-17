#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>

void coordinatesInput(std::vector<std::pair<double, double>> &inputVector) {
    std::cout << "������� ����������, �������� ��������, ���������� ����� ���������� ����:" << std::endl;
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
                else { throw std::exception("����������� ������� �������"); }
                break;
            case ';':
                if (inCoordinateFlag) {
                    // �������� ������ ����� � ����
                    currentPair.first = stod(currentNumber);
                    currentNumber = "";
                } else { throw std::exception("����������� ������� �������"); }

                break;
            case ')':
                if (inCoordinateFlag) {
                    // �������� ������ ����� � ���� � �������� ���� � ������
                    currentPair.second = stod(currentNumber);
                    currentNumber = "";
                    inputVector.push_back(currentPair);
                    inCoordinateFlag = false;
                } else { throw std::exception("����������� ������� �������"); }
                break;
            default:
                if (inCoordinateFlag && (currentInput[i] == '.' || isdigit(currentInput[i]))) {
                    currentNumber += currentInput[i];
                } else if (inCoordinateFlag && currentInput[i] != ' ') {
                    throw std::exception("����������� ������");
                }
                break;
        }
    }
}

//  ���������� true, ���� C ��������� ����� ������� AB, � false � ��������� ������
bool leftOrRight(std::pair<double, double> A, std::pair<double, double> B, std::pair<double, double> C) {

    // ���������� �������� �������-������� AB � ����� x
    auto func{[&B, &A](double x) { return (B.second - A.second) * (x - A.first) / (B.first - A.first) + A.second; }};
    // true, ���� ������ AB ��������� ����� �������, false � ��������� ������
    bool funcDirection = B.first > A.first;

    // true, ���� ����� C ��������� ���� ������� AB, false � ��������� ������
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