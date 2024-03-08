#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

std::map<int, std::map<int, int>> read(const std::string &path) {
    auto resultMap = std::map<int, std::map<int, int>>{};
    std::ifstream in(path);
    if (in.good()) { // ���� ��� ������
        std::string tempString;
        std::vector<int> tempDots;

        std::getline(in, tempString);
        std::istringstream fss(tempString);
        int tempNumber;

        while (fss >> tempNumber) {
            tempDots.push_back(tempNumber);

        }
        for (const auto &dot: tempDots) {
            std::getline(in, tempString);
            std::istringstream ss(tempString);
            ss >> tempNumber;
            if (tempNumber != dot) {
                throw std::exception("������� ������������ ������");
            }
            resultMap.insert({dot, std::map<int, int>{}});
            int count = 0;
            while (ss >> tempNumber) {
                resultMap[dot].insert({tempDots[count++], tempNumber});
            }
            if (resultMap[dot].size() < tempDots.size()) {
                std::string e = "����� ���������� ����� ��� �����";
                e += std::to_string(dot);
                throw std::out_of_range(e);
            }

        }
    }
    in.close();
    return resultMap;
}

std::map<int, int> BreadthFirstSearch(const int rootDot, const std::string &path) {
    auto input = read(path);

    std::map<int, int> distanceMap{};
    for (const auto &[dot, distances]: input) {
        if (dot == rootDot) {
            distanceMap.insert({dot, 0});
        } else {
            distanceMap.insert({dot, -1});
        }
    }

    std::queue<int> allDots{};
    allDots.push(rootDot);

    bool flag = true;
    int currentDistance = 0;

    while (!allDots.empty()) {
        int currentDot = allDots.front();
        allDots.pop();
        for (const auto &[otherDot, distance]: input[currentDot]) {
            if (distance != 0 && distanceMap[otherDot] == -1) {
                distanceMap[otherDot] = distanceMap[currentDot] + 1;
                allDots.push(otherDot);
            }
        }
    }
    return distanceMap;
}

std::vector<std::vector<int>> findAllDots(const std::string &path) {
    std::vector<std::vector<int>> result{};
    auto input = read(path);
    bool flag = true;

    std::vector<int> allDots{};
    for (const auto &dot: input) {
        allDots.push_back(dot.first);
    }

    std::vector<int> tempGoodDots{}; // ������� �� ������ ��������� �����
    auto distanceMap = BreadthFirstSearch(allDots.front(),
                                          path); // ������� map ����� � ���������� �� ���, ������� ������ � ������ �����

    std::vector<int> allGoodDots{};

    // ����� ����������� ���������
    do {
        allDots.clear();
        for (const auto &[dot, distance]: distanceMap) { // ��������� ����� �� ������� � ��� ���������
            if (distance == -1) {
                bool tempFlag = true;
                for (auto const tempGoodDot: allGoodDots) {
                    if (dot == tempGoodDot) {
                        tempFlag = false;
                        break;
                    }
                }
                if (tempFlag) {
                    allDots.push_back(
                            dot); // ��� ��������� ����� - ��, �� ������� �� �� ���������� �� ������� ��������� ������ �����
                }
            } else {
                allGoodDots.push_back(dot);
                tempGoodDots.push_back(
                        dot); // ������� ����� - ��, �� ������� �� ���������� �� ������� ��������� ������ �����
            }
        }

        if (!allDots.empty()) { // ���� ���� �� �����, �� ������� �� �� ����������
            distanceMap = BreadthFirstSearch(allDots.front(),
                                             path); // �� ������ ����� � ������ ������� � �����, �� ������� �� �� ���������� ������
        }

        if (!tempGoodDots.empty()) {

            result.push_back(tempGoodDots); // ���������� ����� � �����, ���� ��� �������� ������ ��������� ���������
            tempGoodDots.clear();
        }
        if (tempGoodDots.empty() && allDots.empty()) {
            flag = false; // ����� �����, ���� ����� �����������
        }
    } while (flag);

    // �������� �� ������� �����, �� ��������� �����
    for (int i = 0; i < result.size() - 1; i++) { // ���������� �������, ����� ������� ����� ���������� �
        bool tempFlag = true;
        for (int j = i + 1; j < result.size(); j++) { // ������� ��������, ������� ���������� ���
            for (int u = 0; u < result[i].size(); u++) { // ������� ��������� ������� ������� ��� ���������
                for (int k = 0; k < result[j].size(); k++) { // � ����������, ������� ������������ � ������ �������
                    if (result[i][u] == result[j][k]) {
                        tempFlag = false;
                        result.erase(result.cbegin() + ((result[i].size() < result[j].size()) ? i : j));
                        break;
                    }
                }
                if (!tempFlag) {
                    break;
                }
            }
            if (!tempFlag) {
                i--;
                break;
            }
        }
    }
    return result;
}


void writeDistances(const std::map<int, int> &distanceMap) {
    std::ofstream out("data/outputDistance.txt");
    for (const auto &[dot, distance]: distanceMap) {
        std::cout << dot << ": " << distance << std::endl;
        out << dot << ": " << distance << std::endl;
    }
    out.close();
}

void writeDots(const std::vector<std::vector<int>> &distanceMap) {
    std::ofstream out("data/outputDots.txt");
    for (const auto &dots: distanceMap) {
        std::cout << dots.size() << ": ";
        out << dots.size() << ": ";
        for (const auto dot: dots) {
            std::cout << dot;
            out << dot;
            if (dot != dots.back()) {
                std::cout << ", ";
                out << ", ";
            }
        }
        std::cout << std::endl;
        out << std::endl;
    }
    out.close();
}

