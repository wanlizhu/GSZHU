#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    /* Distance and closest-point queries.
     * A DCPQuery-base class B must define a B::Result struct with member 'T distance'. 
     * A DCPQuery-derived class D must also derive a D::Result from B:Result but may have no members.
     * The idea is to allow Result to store closest-point information in addition to the distance.
     * The operator() is non-const to allow DCPQuery to store and modify private state 
     * that supports the query.
     */
    template<typename T, typename SHAPE0, typename SHAPE1>
    class DCPQuery
    {
    public:
        struct Result {};

        Result operator()(const SHAPE0& shape0, const SHAPE1& shape1);
    };

}