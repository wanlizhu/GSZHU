#pragma once

namespace CGA
{
    template<typename SHAPE0, typename SHAPE1>
    class DCPQuery
    {
    public:
        // The idea is to allow ComputeResult to store closest-point information in addition to the distance.  
        // The operator() is non-const to allow DCPQuery to store and modify private state that supports the query.
        struct ComputeResult {};

        ComputeResult operator()(const SHAPE0& shape0, const SHAPE1& shape1) { static_assert(false); }
    };
}