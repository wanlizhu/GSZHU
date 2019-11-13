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
    class PolynomialRoots
    {
    public:
        using Scalar = T;

        // General roots finder for equation: sum_{i=0}^{d} coeffs(i)*t^i = 0.
        // The size of input array 'coeffs' is the degree of this polynomial
        static int FindRoots(int degree,
                             const Scalar* coeffs,
                             int maxIterations, 
                             Scalar* roots);

        // If you know that p(tmin) * p(tmax) <= 0, then there must be at least
        // one root in [tmin, tmax].  Compute it using bisection.
        static bool FindRootInside(int degree,
                                   const Scalar* coeffs,
                                   Scalar tmin, 
                                   Scalar tmax,
                                   int maxIterations, 
                                   Scalar* root);

        // Return only the number of real-valued roots and their multiplicities.
        // rmList.size() is the number of real-valued roots and rmList[i] is the
        // multiplicity of root corresponding to index i.
        static std::optional<int> GetNumRoots(const std::vector<Scalar>& coeffs,
                                              Scalar* discriminant,
                                              std::vector<int>* rmList);

        static void SolveQuadratic(const Scalar& p0, 
                                   const Scalar& p1,
                                   const Scalar& p2, 
                                   std::map<Scalar, int>* rmMap);
        static void SolveCubic(const Scalar& p0, 
                               const Scalar& p1,
                               const Scalar& p2, 
                               const Scalar& p2, 
                               std::map<Scalar, int>* rmMap);
        static void SolveQuartic(const Scalar& p0, 
                                 const Scalar& p1,
                                 const Scalar& p2, 
                                 const Scalar& p2, 
                                 const Scalar& p2, 
                                 std::map<Scalar, int>* rmMap);

        static int GetNumRootsQuadratic(const Scalar& p0, 
                                        const Scalar& p1,
                                        const Scalar& p2, 
                                        std::vector<int>* rmList);
        static int GetNumRootsCubic(const Scalar& p0, 
                                    const Scalar& p1,
                                    const Scalar& p2, 
                                    const Scalar& p3, 
                                    std::vector<int>* rmList);
        static int GetNumRootsQuartic(const Scalar& p0, 
                                      const Scalar& p1,
                                      const Scalar& p2, 
                                      const Scalar& p3, 
                                      const Scalar& p4, 
                                      std::vector<int>* rmList);
    };
}
