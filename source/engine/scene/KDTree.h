#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <memory>
#include <functional>

namespace NTGS {
    template<typename T, typename COMPARE = std::less<T>>
    class BoundedPriorityQueue {
    public:
        BoundedPriorityQueue() = delete;
        BoundedPriorityQueue(size_t bound) : mBound(bound) { mElements.reserve(bound + 1); }

        const T& Back() const { return mElements.back(); }
        const T& operator[](size_t index) const { return mElements[index]; }
        size_t Size() const { return mElements.size(); }
        void Push(const T& val) { 
            auto it = std::find_if(std::begin(mElements), std::end(mElements), [&](const T& element) {
                return COMPARE()(val, element);
            });
            if (mElements.size() > mBound)
                mElements.resize(mBound);
        }
        
    private:
        size_t mBound = -1;
        std::vector<T> mElements;
    };


    template<typename _POINT>
    class KDTree {
        struct Node {
            size_t index = -1;
            std::shared_ptr<Node> children[2] = { nullptr };
            int axis = -1;
        };
        using NodePtr = std::shared_ptr<Node>;
        using ConstNodePtr = std::shared_ptr<const Node>;
        using KNNQueue = BoundedPriorityQueue<std::pair<double, int>>;

    public:
        typedef size_t INDEX;
        typedef _POINT POINT;

        KDTree();
        KDTree(const std::vector<POINT>& points);
        virtual ~KDTree();

        void Build(const std::vector<POINT>& points);
        void Clear();
        bool IsValid() const;

        INDEX FindNearest(const POINT& ref, double* distMin = nullptr) const;
        std::vector<INDEX> FindKNearest(const POINT& ref, int K) const;
        std::vector<INDEX> FindInside(const POINT& ref, double radius) const;

    private:
        static double GetDistance(const POINT& p, const POINT& q);
        NodePtr BuildSubtree(INDEX* indices, size_t count, int depth);
        bool IsValidSubtree(ConstNodePtr node, int depth);
        void FindNearestSubtree(const POINT& ref, ConstNodePtr node, INDEX* index, double* dist) const;
        void FindKNearestSubtree(const POINT& ref, ConstNodePtr node, KNNQueue& queue, int K) const;
        void FindInsideSubtree(const POINT& ref, ConstNodePtr node, std::vector<INDEX>& indices, double radius) const;

    private:
        NodePtr mRoot;
        std::vector<POINT> mPoints;
    };
}