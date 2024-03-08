#pragma once

#include "gtest/gtest.h"
#include "func.h"


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