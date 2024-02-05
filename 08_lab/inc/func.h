#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::map<int, std::map<int, int>> read(const std::string &path) {
    auto resultMap = std::map<int, std::map<int, int>>{};
    std::ifstream in(path);
    if (in.good()) { // если все хорошо
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
                throw std::exception("Введены некорректные данные");
            }
            resultMap.insert({dot, std::map<int, int>{}});
            int count = 0;
            while (ss >> tempNumber) {
                resultMap[dot].insert({tempDots[count++], tempNumber});
            }
            if (resultMap[dot].size() < tempDots.size()) {
                std::string e = "Малое количество путей для точки";
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
        for (const auto &[dot, distance]: distanceMap) { // перебор по словарю с точками и дистанциями до них
            if (distance == currentDistance) { // если нашли точку с необходимой дистанцией до нее
                for (const auto &[destinationDot, destinationDistance]: input[dot]) { // то делаем перебор по точкам, в которые можно попасть из текущей точки
                    if (destinationDistance > 0 &&
                        // если для выбранной точки: минимальная дистанция положительная и выполняется одно из двух условий
                        (distanceMap[destinationDot] > currentDistance + destinationDistance ||
                         // 1) минимальная дистанция до выбранной точки больше полученной сейчас
                         distanceMap[destinationDot] == -1)) {
                        // 2) дистанция до точки не была найдена
                        distanceMap[destinationDot] = currentDistance +
                                                      destinationDistance; // тогда присваиваем дистанцию до выбранной точки с отсчетом от прошлой точки
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

