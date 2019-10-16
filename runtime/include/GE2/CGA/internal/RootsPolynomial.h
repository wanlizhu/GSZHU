#pragma once

#include "GE2/Utilities.h"

// The Find functions return the number of roots, if any, and this number
// of elements of the outputs are valid.  If the polynomial is identically
// zero, Find returns 1.
//
// Some root-bounding algorithms for real-valued roots are mentioned next for
// the polynomial p(t) = c[0] + c[1]*t + ... + c[d-1]*t^{d-1} + c[d]*t^d.
//
// 1. The roots must be contained by the interval [-M,M] where
//   M = 1 + max{|c[0]|, ..., |c[d-1]|}/|c[d]| >= 1
// is called the Cauchy bound.
//
// 2. You may search for roots in the interval [-1,1].  Define
//   q(t) = t^d*p(1/t) = c[0]*t^d + c[1]*t^{d-1} + ... + c[d-1]*t + c[d]
// The roots of p(t) not in [-1,1] are the roots of q(t) in [-1,1].
//
// 3. Between two consecutive roots of the derivative p'(t), say, r0 < r1,
// the function p(t) is strictly monotonic on the open interval (r0,r1).
// If additionally, p(r0) * p(r1) <= 0, then p(x) has a unique root on
// the closed interval [r0,r1].  Thus, one can compute the derivatives
// through order d for p(t), find roots for the derivative of order k+1,
// then use these to bound roots for the derivative of order k.
//
// 4. Sturm sequences of polynomials may be used to determine bounds on the
// roots.  This is a more sophisticated approach to root bounding than item 3.
// Moreover, a Sturm sequence allows you to compute the number of real-valued
// roots on a specified interval.

namespace GE2::CGA
{
    class GE2_IMPEXP RootsPolynomial final
    {
    public:
        // Low-degree root finders
        static void SolveQuadratic(const double& p0, const double& p1, const double& p2,
                                   std::map<double, int>& rmMap);

        static void SolveCubic(const double& p0, const double& p1, const double& p2, const double& p3,
                               std::map<double, int>& rmMap);

        static void SolveQuartic(const double& p0, const double& p1, const double& p2, const double& p3, const double& p4,
                                 std::map<double, int>& rmMap);

        // Return only the number of real-valued roots and their multiplicities.
        // info.size() is the number of real-valued roots and info[i] is the
        // multiplicity of root corresponding to index i.
        static void GetRootInfoQuadratic(const double& p0, const double& p1, const double& p2, 
                                         std::vector<int>& info);

        static void GetRootInfoCubic(const double& p0, const double& p1, const double& p2, const double& p3,
                                     std::vector<int>& info);

        static void GetRootInfoQuartic(const double& p0, const double& p1, const double& p2, const double& p3, const double& p4,
                                       std::vector<int>& info);

        // Find the roots on (-infinity,+infinity).
        static int Find(int degree, const double* c, unsigned int maxIterations, double* roots);

        // If you know that p(tmin) * p(tmax) <= 0, then there must be at least one root in [tmin, tmax].  
        // Compute it using bisection.
        static bool Find(int degree, const double* c, double tmin, double tmax, 
                         unsigned int maxIterations, double& root);
    
    private:
        // Support for the Solve* functions.
        static void SolveDepressedQuadratic(double const& c0,
                                            std::map<double, int>& rmMap);

        static void SolveDepressedCubic(double const& c0, double const& c1,
                                        std::map<double, int>& rmMap);

        static void SolveDepressedQuartic(double const& c0, double const& c1,
                                          double const& c2, std::map<double, int>& rmMap);

        static void SolveBiquadratic(double const& c0, double const& c2,
                                     std::map<double, int>& rmMap);

        // Support for the GetNumRoots* functions.
        static void GetRootInfoDepressedQuadratic(double const& c0,
                                                  std::vector<int>& info);

        static void GetRootInfoDepressedCubic(double const& c0,
                                              double const& c1, std::vector<int>& info);

        static void GetRootInfoDepressedQuartic(double const& c0,
                                                double const& c1, double const& c2, std::vector<int>& info);

        static void GetRootInfoBiquadratic(double const& c0,
                                           double const& c2, std::vector<int>& info);

        // Support for the Find functions.
        static int FindRecursive(int degree, double const* c, double tmin, double tmax,
                                 unsigned int maxIterations, double* roots);

        static double Evaluate(int degree, double const* c, double t);
    };
}