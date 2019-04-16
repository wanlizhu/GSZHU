#pragma once

namespace NTGS {
    template<typename POINT>
    class KDTree {
        struct KDNode {
            size_t mPointIndex = 0;
            int mSplitDimension = 0;
            std::unique_ptr<KDNode> mChildren[2];

            KDNode(size_t ind) : mPointIndex(ind) {}
        };
        using IndexArray = std::vector<size_t>;
        using IndexArrayIterator = typename std::vector<size_t>::iterator;
    public:
        KDTree();
        KDTree(const std::vector<POINT>& points);

        void Create(const std::vector<POINT>& points);
        size_t FindMidPoint(int dim);
        void InsertPoint(const POINT& point);
        void DeletePoint(size_t id);
        POINT& GetPoint(size_t id);
        const POINT& GetPoint(size_t id) const;
        template<typename BOUND> int SelectPoints(const BOUND& boundary, std::vector<size_t>* points);
        template<typename FUNC> void Traversal(FUNC func);
        
    private:
        std::shared_ptr<KDNode> CreateSubTree(IndexArray& indices, const IndexArrayIterator& begin, const IndexArrayIterator& end);
        
    private:
        std::shared_ptr<KDNode> mpRootNode;
        std::vector<POINT> mPointsArray;
    };
}