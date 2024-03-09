#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

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

    [[nodiscard]] const std::vector<Dot *> *neighbours() const {
        return &_neighboursDots;
    }

    [[nodiscard]] unsigned int neighboursCount() const {
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

    void
    recursiveDepthFirstSearch(unsigned int currentDotValue, unsigned int endDotValue,
                              std::vector<unsigned int> *visited = new std::vector<unsigned int>,
                              std::vector<std::vector<unsigned int>> *routes = new std::vector<std::vector<unsigned int>>) const {
        visited->push_back(currentDotValue);

        if (currentDotValue == endDotValue) {
            routes->push_back(*visited);
        } else {
            const auto neighbours = this->get(currentDotValue)->neighbours();
            for (const auto *neighbour: *neighbours) {
                const auto nextDotValue = neighbour->value();
                if (std::find(visited->cbegin(), visited->cend(), nextDotValue) == visited->cend()) {
                    recursiveDepthFirstSearch(nextDotValue, endDotValue, visited, routes);
                }
            }
        }
        visited->pop_back();
    }

public:

    [[nodiscard]] bool isDot(unsigned int value) const {
        auto it = std::find_if(_dots.cbegin(), _dots.cend(),
                               [&value](Dot *currentDot) { return currentDot->value() == value; });
        if (it == _dots.cend()) {
            return false;
        }
        return true;
    }

    [[nodiscard]] Dot *get(unsigned int value) const {
        if (!isDot(value)) {
            throw std::exception("Ёлемент не найден");
        }
        return _dots[std::find_if(_dots.cbegin(), _dots.cend(),
                                  [&value](Dot *currentDot) { return currentDot->value() == value; }) - _dots.cbegin()];
    }

    [[nodiscard]] unsigned int size() const {
        return _dots.size();
    }

    void addDot(unsigned int value) {
        try {
            Dot *temp = get(value);
        } catch (const std::exception &error) {
            _dots.push_back(new Dot(value));
        }
    }

    void addEdge(unsigned firstDotValue, unsigned secondDotValue) {
        addDot(firstDotValue);
        addDot(secondDotValue);
        get(firstDotValue)->addLink(get(secondDotValue));
    }

    [[nodiscard]] std::vector<std::vector<unsigned int>> *
    depthFirstSearch(unsigned int startDotValue, unsigned int endDotValue) const {
        auto *visited = new std::vector<unsigned int>;
        auto *routes = new std::vector<std::vector<unsigned int>>;

        recursiveDepthFirstSearch(startDotValue, endDotValue, visited, routes);
        return routes;
    }
};

Graph *read(const std::string &path) {
    auto result = new Graph();

    std::ifstream in(path);
    if (in.good()) { // если все хорошо
        std::string tempString;

        std::getline(in, tempString);
        std::istringstream fss(tempString);

        int value;
        while (fss >> value) {
            result->addDot(value);
        }

        for (int i = 0; i < result->size(); i++) {
            std::getline(in, tempString);
            std::istringstream ss(tempString);
            ss >> value;
            if (!result->isDot(value)) {
                throw std::exception("¬ведены некорректные данные");
            }

            unsigned int currentDotValue = value;
            unsigned int otherDotValue = 1;
            while (ss >> value) {
                if (value) result->addEdge(currentDotValue, otherDotValue);
                otherDotValue++;
            }
        }
    }
    in.close();
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

