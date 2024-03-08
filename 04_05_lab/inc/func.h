#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

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

class Dot {
private:
    unsigned int _value;
    std::vector<Dot *> _neighboursDots;

    void deleteLink(Dot *otherDot) {
        _neighboursDots.erase(std::find(_neighboursDots.cbegin(), _neighboursDots.cend(), otherDot));
    }

public:

    explicit Dot(unsigned int value) : _value(value) {}

    ~Dot() {
        for (auto *otherDot: _neighboursDots) {
            otherDot->deleteLink(this);
        }
    }

    [[nodiscard]] unsigned int value() const {
        return _value;
    }

    [[nodiscard]]unsigned int neighboursCount(){
        return _neighboursDots.size();
    }

    [[nodiscard]] bool isLinked(const Dot *otherDot) const {
        return std::find(_neighboursDots.cbegin(), _neighboursDots.cend(), otherDot) != _neighboursDots.cend() &&
               std::find(otherDot->_neighboursDots.cbegin(), otherDot->_neighboursDots.cend(), this) !=
               otherDot->_neighboursDots.cend();
    }

    void addLink(Dot *otherDot) {
        if (!this->isLinked(otherDot)) {
            _neighboursDots.push_back(otherDot);
        }
        if (!otherDot->isLinked(this)) {
            otherDot->_neighboursDots.push_back(this);
        }
    }
};

class Graph {
private:
    std::vector<Dot *> _dots;
public:
    [[nodiscard]] Dot *get(unsigned int value) {
        auto it = std::find_if(_dots.cbegin(), _dots.cend(),
                               [&value](Dot *currentDot) { return currentDot->value() == value; });
        if (it == _dots.cend()) {
            throw std::exception("Элемент не найден");
        }
        return _dots[it - _dots.cbegin()];
    }

    unsigned int size(){
        return _dots.size();
    }

    void addDot(unsigned int value) {
        try {
            Dot *temp = get(value);
        } catch (const std::exception &error) {
            _dots.push_back(new Dot(value));
        }
    }

    void addEdge(unsigned firstValue, unsigned secondValue) {
        addDot(firstValue);
        addDot(secondValue);
        get(firstValue)->addLink(get(secondValue));
    }
};

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

