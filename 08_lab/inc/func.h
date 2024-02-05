#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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
//            tempDots.clear();
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

std::map<int, int> Dijkstra(const int rootDot, const std::string &path) {
    auto input = read(path);
    std::map<int, int> distanceMap{};
    for (const auto &[dot, distances]: input) {
        if (dot == rootDot) {
            distanceMap.insert({dot, 0});
        } else {
            distanceMap.insert({dot, -1});
        }
    }
    bool flag = true;
    int currentDistance = 0;
    while (flag) {
        flag = false;
        for (const auto &[dot, distance]: distanceMap) { // ������� �� ������� � ������� � ����������� �� ���
            if (distance == currentDistance) { // ���� ����� ����� � ����������� ���������� �� ���
                for (const auto &[destinationDot, destinationDistance]: input[dot]) { // �� ������ ������� �� ������, � ������� ����� ������� �� ������� �����
                    if (destinationDistance > 0 &&
                        // ���� ��� ��������� �����: ����������� ��������� ������������� � ����������� ���� �� ���� �������
                        (distanceMap[destinationDot] > currentDistance + destinationDistance ||
                         // 1) ����������� ��������� �� ��������� ����� ������ ���������� ������
                         distanceMap[destinationDot] == -1)) {
                        // 2) ��������� �� ����� �� ���� �������
                        distanceMap[destinationDot] = currentDistance +
                                                      destinationDistance; // ����� ����������� ��������� �� ��������� ����� � �������� �� ������� �����
                    }
                }
            }
        }
        currentDistance++;

        std::map<int, int> maxDistances{};
        std::pair<int, int> maxDistanceDot{0, 0};
        for (const auto &[dot, distance]: distanceMap) {
            if (distance > maxDistanceDot.second) {
                maxDistanceDot.first = dot;
                maxDistanceDot.second = distance;
            }
        }
        for (const auto &dot: distanceMap) {
            if (dot.second == maxDistanceDot.second) {
                maxDistances.insert(dot);
            }
        }
        for (const auto &[dot, distance]: maxDistances) {
            bool tempFlag = true;
            for (const auto &targetDistance: input[dot]) {
                if (targetDistance.second > 0) {
                    for (const auto &distanceDot: distanceMap) {
                        if (targetDistance.first == distanceDot.first && distanceDot.second == -1) {
                            flag = true;
                            tempFlag = false;
                            break;
                        }
                    }
                    if (!tempFlag) {
                        break;
                    }
                }
            }
            if (!tempFlag) {
                break;
            }
        }
    }
    return distanceMap;
}

void writeDistances(const std::map<int, int> &distanceMap) {
    std::ofstream out("data/outputDistance.txt");
    for (const auto &[dot, distance]: distanceMap) {
        std::cout << dot << ": " << distance << std::endl;
        out << dot << ": " << distance << std::endl;
    }
    out.close();
}

