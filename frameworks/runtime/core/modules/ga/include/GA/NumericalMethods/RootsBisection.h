#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <cmath>
#include <type_traits>
#include <optional>

namespace GA
{
    template<typename T>
    class RootsBisection
    {
    public:
        using Scalar = T;

        static bool FindRootInside(const std::function<Scalar(Scalar)>& F, 
                                   Scalar t0,
                                   Scalar t1,
                                   int maxIterations,
                                   Scalar* root);
    };
}