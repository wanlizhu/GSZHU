#include "GE2/CGA/details/RootsPolynomial.h"

namespace CGA
{
    void RootsPolynomial::SolveQuadratic(const double& p0, const double& p1, const double& p2,
                                         std::map<double, int>& rmMap)
    {
        double const rat2 = 2;
        double q0 = p0 / p2;
        double q1 = p1 / p2;
        double q1half = q1 / rat2;
        double c0 = q0 - q1half * q1half;

        std::map<double, int> rmLocalMap;
        SolveDepressedQuadratic(c0, rmLocalMap);

        rmMap.clear();
        for (auto& rm : rmLocalMap)
        {
            double root = rm.first - q1half;
            rmMap.insert(std::make_pair(root, rm.second));
        }
    }
}