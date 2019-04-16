#pragma once

namespace NTGS {
    template<typename DATA>
    class KDTree {
        struct KDNode {
            size_t mDataIndex = 0;
            int mSplitDimension = 0;
            std::unique_ptr<KDNode> mChildren[2];

            KDNode(size_t ind) : mDataIndex(ind) {}
        };
    public:
        KDTree();
        KDTree(const std::vector<DATA>& points);

        void Create(const std::vector<DATA>& points);
        size_t FindMid(int dim);
        void InsertData(const DATA& point);
        void DeleteData(size_t id);
        DATA& GetPoint(size_t id);
        const DATA& GetPoint(size_t id) const;
        template<typename BOUND> int SelectPoints(const BOUND& boundary, std::vector<size_t>* points);
        template<typename FUNC> void Traversal(FUNC func);
        
    private:
        std::shared_ptr<KDNode> CreateSubTree(IndexArray& indices, const IndexArrayIterator& begin, const IndexArrayIterator& end);
        
    private:
        std::shared_ptr<KDNode> mpRootNode;
        std::vector<DATA> mDataArray;
    };
}