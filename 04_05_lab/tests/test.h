#pragma once

#include "gtest/gtest.h"
#include <func.h>


TEST(DotTest, Links) {
    Dot *firstDot = new Dot(1);
    Dot *secondDot = new Dot(2);
    Dot *thirdDot = new Dot(3);
    firstDot->addLink(thirdDot);
    thirdDot->addLink(secondDot);
    bool result = !firstDot->isLinked(secondDot) && secondDot->isLinked(thirdDot) &&
                  firstDot->isLinked(thirdDot);
    ASSERT_TRUE(result);
}

TEST(DotTest, Destructor) {
    Dot *firstDot = new Dot(1);
    Dot *secondDot = new Dot(2);
    firstDot->addLink(secondDot);
    delete secondDot;
    ASSERT_EQ(0, firstDot->neighboursCount());
}

TEST(GraphTest, AddAndGetDot) {
    auto array = new Graph();
    array->addDot(1);
    array->addDot(2);
    array->addDot(1);

    ASSERT_EQ(array->size(), 2);
    ASSERT_EQ(array->get(1)->value(), 1);
    ASSERT_EQ(array->get(2)->value(), 2);
}

TEST(GraphTest, Edges) {
    auto array = new Graph();
    array->addDot(1);
    array->addDot(2);
    array->addEdge(1, 2);
    ASSERT_TRUE(array->get(1)->isLinked(array->get(2)));
}

TEST(GraphTest, FileRead1) {
//   inputData:
//      1 2 3
//    1 0 1 0
//    2 1 0 1
//    3 0 1 0

    auto testGraph = read("tests/testInput1.txt");
    ASSERT_TRUE(testGraph->get(1)->isLinked(testGraph->get(2)));
    ASSERT_TRUE(!testGraph->get(1)->isLinked(testGraph->get(3)));
    ASSERT_TRUE(testGraph->get(3)->isLinked(testGraph->get(2)));
}

TEST(GraphTest, DFS1) {
//   inputData:
//      1 2 3
//    1 0 1 0
//    2 1 0 1
//    3 0 1 0

    auto testGraph = read("tests/testInput1.txt");
    auto *result = testGraph->depthFirstSearch(1, 3);
    ASSERT_EQ((*result)[0][0], 1);
    ASSERT_EQ((*result)[0][1], 2);
    ASSERT_EQ((*result)[0][2], 3);
    ASSERT_EQ((*result).size(), 1);
}

TEST(GraphTest, FileRead2) {
//   inputData:
//      1 2 3
//    1 0 1 1
//    2 1 0 1
//    3 1 1 0

    auto testGraph = read("tests/testInput2.txt");
    ASSERT_TRUE(testGraph->get(1)->isLinked(testGraph->get(2)));
    ASSERT_TRUE(testGraph->get(1)->isLinked(testGraph->get(3)));
    ASSERT_TRUE(testGraph->get(3)->isLinked(testGraph->get(2)));
}

TEST(GraphTest, DFS2) {
//   inputData:
//      1 2 3
//    1 0 1 1
//    2 1 0 1
//    3 1 1 0

    auto testGraph = read("tests/testInput2.txt");
    auto *result = testGraph->depthFirstSearch(1, 3);


    ASSERT_EQ((*result)[0][0], 1);
    ASSERT_EQ((*result)[0][1], 2);
    ASSERT_EQ((*result)[0][2], 3);

    ASSERT_EQ((*result)[1][0], 1);
    ASSERT_EQ((*result)[1][1], 3);

    ASSERT_EQ((*result).size(), 2);
}