#pragma once

#include "KDTree.h"

namespace NTGS {
    template<typename POINT>
    KDTree<POINT>::KDTree() 
        : mRoot(nullptr) 
    {}

    template<typename POINT>
    KDTree<POINT>::KDTree(const std::vector<POINT>& points)
        : mRoot(nullptr) {
        Build(points);
    }

    template<typename POINT>
    KDTree<POINT>::~KDTree() {
        Clear();
    }

    template<typename POINT>
    void KDTree<POINT>::Build(const std::vector<POINT>& points) {
        Clear();

        mPoints = points;
        std::vector<INDEX> indices(points.size());
        std::iota(std::begin(indices), std::end(indices), 0);

        mRoot = BuildSubtree(indices.data(), points.size(), 0);
    }

    template<typename POINT>
    void KDTree<POINT>::Clear() {
        mRoot = NodePtr(nullptr);
        mPoints.clear();
    }

    template<typename POINT>
    bool KDTree<POINT>::IsValid() const {
        return IsValidSubtree(mRoot, 0);
    }

    template<typename POINT>
    typename KDTree<POINT>::INDEX KDTree<POINT>::FindNearest(const POINT& ref, double* distMin) const {
        INDEX index = INDEX(-1);
        double myDistMin = std::numeric_limits<double>::max();

        FindNearestSubtree(ref, mRoot, &index, &myDistMin);

        if (distMin) *distMin = myDistMin;

        return index;
    }

    template<typename POINT>
    std::vector<typename KDTree<POINT>::INDEX> KDTree<POINT>::FindKNearest(const POINT& ref, size_t K) const {
        KNNQueue queue(K);
        FindKNearestSubtree(ref, mRoot, queue, K);

        std::vector<INDEX> indices(queue.Size());
        for (size_t i = 0; i < queue.Size(); i++)
            indices[i] = queue[i].second;
        return indices;
    }

    template<typename POINT>
    std::vector<typename KDTree<POINT>::INDEX> KDTree<POINT>::FindInside(const POINT& ref, double radius) const {
        std::vector<INDEX> indices;
        FindInsideSubtree(ref, mRoot, indices, radius);
        return indices;
    }

    template<typename POINT>
    typename KDTree<POINT>::NodePtr KDTree<POINT>::BuildSubtree(typename KDTree<POINT>::INDEX* indices, size_t count, int depth) {
        if (count <= 0)
            return nullptr;

        const int axis = depth % POINT::DIMENSION;
        const size_t mid = (count - 1) / 2;

        std::nth_element(indices, indices + mid, indices + count, [&] (size_t lhs, size_t rhs) {
            return mPoints[lhs][axis] < mPoints[rhs][axis];
        });

        NodePtr node = std::make_shared<Node>();
        node->index = indices[mid];
        node->axis = axis;
        node->children[0] = BuildSubtree(indices, mid, depth + 1);
        node->children[1] = BuildSubtree(indices + mid + 1, count - mid - 1, depth + 1);

        return node;
    }

    template<typename POINT>
    bool KDTree<POINT>::IsValidSubtree(ConstNodePtr node, int depth) const {
        if (node == nullptr)
            return true;

        const int axis = node->axis;
        ConstNodePtr nodeL = node->children[0];
        ConstNodePtr nodeR = node->children[1];
        
        if (nodeL && nodeR) {
            if (mPoints[node->index][axis] < mPoints[nodeL->index][axis])
                return false;

            if (mPoints[node->index][axis] > mPoints[nodeR->index][axis])
                return false;
        }

        if (nodeL && !IsValidSubtree(nodeL, depth + 1))
            return false;

        if (nodeR && !IsValidSubtree(nodeR, depth + 1))
            return false;
    }

    template<typename POINT>
    double KDTree<POINT>::GetDistance(const POINT& p, const POINT& q) {
        double dist = 0;
        for (size_t i = 0; i < POINT::DIMENSION; i++)
            dist += (p[i] - q[i]) * (p[i] - q[i]);
        return sqrt(dist);
    }

    template<typename POINT>
    void KDTree<POINT>::FindNearestSubtree(const POINT& ref, ConstNodePtr node, INDEX* index, double* distMin) const {
        if (node == nullptr)
            return;

        const POINT& train = mPoints[node->index];
        const double dist = GetDistance(ref, train);
        
        if (dist < *distMin) {
            *distMin = dist;
            *index = node->index;
        }

        const int axis = node->axis;
        const int child = ref[axis] < train[axis] ? 0 : 1;
        FindNearestSubtree(ref, node->children[child], index, distMin);

        const double diff = fabs(ref[axis] - train[axis]);
        if (diff < *distMin)
            FindNearestSubtree(ref, node->children[1 - child], index, distMin);
    }

    template<typename POINT>
    void KDTree<POINT>::FindKNearestSubtree(const POINT& ref, ConstNodePtr node, KNNQueue& queue, size_t K) const {
        if (node == nullptr)
            return;

        const POINT& train = mPoints[node->index];
        const double dist = GetDistance(ref, train);
        queue.Push(std::make_pair(dist, node->index));

        const int axis = node->axis;
        const int child = ref[axis] < train[axis] ? 0 : 1;
        FindKNearestSubtree(ref, node->children[child], queue, K);

        const double diff = fabs(ref[axis] - train[axis]);
        if (queue.Size() < K || diff < queue.Back().first)
            FindKNearestSubtree(ref, node->children[1 - child], queue, K);
    }

    template<typename POINT>
    void KDTree<POINT>::FindInsideSubtree(const POINT& ref, ConstNodePtr node, std::vector<INDEX>& indices, double radius) const {
        if (node == nullptr)
            return;

        const POINT& train = mPoints[node->index];
        const double dist = GetDistance(ref, train);
        if (dist < radius)
            indices.push_back(node->index);

        const int axis = node->axis;
        const int child = ref[axis] < train[axis] ? 0 : 1;
        FindInsideSubtree(ref, node->children[child], indices, radius);

        const double diff = fabs(ref[axis] - train[axis]);
        if (diff < radius)
            FindInsideSubtree(ref, node->children[1 - child], indices, radius);
    }
}
