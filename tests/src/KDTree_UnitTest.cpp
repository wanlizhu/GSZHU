#include <gtest/gtest.h>
#include "scene/KDTree.impl.h"

#include <iostream>
#include <array>
#include <vector>

class MyPoint : public std::array<double, 2> {
public:
    static const int DIMENSION = 2;

    MyPoint() {}
    MyPoint(double x, double y) {
        (*this)[0] = x;
        (*this)[1] = y;
    }
};

using namespace NTGS;

TEST(NTGS_Scene, KDTree_TEST) {
    const int width = 500;
    const int height = 500;
    const int npoints = 100;

    std::vector<MyPoint> points(npoints);
    for (size_t i = 0; i < npoints; i++) {
        const double x = rand() % width;
        const double y = rand() % height;
        points[i] = MyPoint(x, y);
    }

    KDTree<MyPoint> kdtree(points);

    EXPECT_TRUE(kdtree.IsValid());
}
