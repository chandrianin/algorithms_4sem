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

std::map<int, int> depthFirstSearch(const int rootDot, const std::string &path) {
    auto input = read(path);
    std::map<int, int> distanceMap{};
    for (const auto &[dot, distances]: input) {
        if (dot == rootDot) {
            distanceMap.insert({dot, 0});
        } else {
            distanceMap.insert({dot, -1});
        }
    }

//    bool flag = true;
//    int currentDistance = 0;
//    while (flag) {
//        flag = false;
//        for (const auto &[dot, distance]: distanceMap) { // перебор по словарю с точками и дистанциями до них
//            if (distance == currentDistance) { // если нашли точку с необходимой дистанцией до нее
//                for (const auto &[destinationDot, destinationDistance]: input[dot]) { // то делаем перебор по точкам, в которые можно попасть из текущей точки
//                    if (destinationDistance > 0 &&
//                        // если для выбранной точки: минимальная дистанция положительная и выполняется одно из двух условий
//                        (distanceMap[destinationDot] > currentDistance + destinationDistance ||
//                         // 1) минимальная дистанция до выбранной точки больше полученной сейчас
//                         distanceMap[destinationDot] == -1)) {
//                        // 2) дистанция до точки не была найдена
//                        distanceMap[destinationDot] = currentDistance +
//                                                      destinationDistance; // тогда присваиваем дистанцию до выбранной точки с отсчетом от прошлой точки
//                    }
//                }
//            }
//        }
//        currentDistance++;
//
//        std::map<int, int> maxDistances{};
//        std::pair<int, int> maxDistanceDot{0, 0};
//        for (const auto &[dot, distance]: distanceMap) {
//            if (distance > maxDistanceDot.second) {
//                maxDistanceDot.first = dot;
//                maxDistanceDot.second = distance;
//            }
//        }
//        for (const auto &dot: distanceMap) {
//            if (dot.second == maxDistanceDot.second) {
//                maxDistances.insert(dot);
//            }
//        }
//        for (const auto &[dot, distance]: maxDistances) {
//            bool tempFlag = true;
//            for (const auto &targetDistance: input[dot]) {
//                if (targetDistance.second > 0) {
//                    for (const auto &distanceDot: distanceMap) {
//                        if (targetDistance.first == distanceDot.first && distanceDot.second == -1) {
//                            flag = true;
//                            tempFlag = false;
//                            break;
//                        }
//                    }
//                    if (!tempFlag) {
//                        break;
//                    }
//                }
//            }
//            if (!tempFlag) {
//                break;
//            }
//        }
//    }
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

    std::vector<int> tempGoodDots{}; // вектора из группы связанных точек
    auto distanceMap = depthFirstSearch(allDots.front(),
                                        path); // создаем map точек и расстояния до них, начиная отсчет с первой точки

    std::vector<int> allGoodDots{};
    do {
        allDots.clear();
        for (const auto &[dot, distance]: distanceMap) { // разделяем точки на хорошие и все остальные
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
                            dot); // все остальные точки - те, до которых мы не дотянулись от прошлой выбранной первой точки
                }
            } else {
                allGoodDots.push_back(dot);
                tempGoodDots.push_back(
                        dot); // хорошие точки - те, до которых мы дотянулись от прошлой выбранной первой точки
            }
        }

        if (!allDots.empty()) { // если есть те точки, до которых мы не дотянулись
            distanceMap = depthFirstSearch(allDots.front(),
                                           path); // то делаем поиск в ширину начиная с точки, до которой мы не дотянулись раньше
        }

        if (!tempGoodDots.empty()) {

            result.push_back(tempGoodDots); // добавление точек в ответ, если они образуют группу компонент связности
            tempGoodDots.clear();
        }
        if (tempGoodDots.empty() && allDots.empty()) {
            flag = false; // конец цикла, если точки закончились
        }
    } while (flag);


    for (int i = 0; i < result.size() - 1; i++) { // перебирает векторы, точки которых будем сравнивать с
        bool tempFlag = true;
        for (int j = i + 1; j < result.size(); j++) { // точками векторов, которые перебираем тут
            for (int u = 0; u < result[i].size(); u++) { // перебор элементов первого вектора для сравнения
                for (int k = 0; k < result[j].size(); k++) { // с элементами, которые перебираются в другом векторе
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

