#include "GE2/CGA/details/RootsPolynomial.h"

namespace CGA
{
    // Support for the Solve* functions.
    void RootsPolynomial::SolveDepressedQuadratic(const double& c0,
                                                  std::map<double, int>& rmMap)
    {
        const double zero = 0;
        if (c0 < zero)
        {
            // Two simple roots.
            double root1 = (double)std::sqrt(-(double)c0);
            double root0 = -root1;
            rmMap.insert(std::make_pair(root0, 1));
            rmMap.insert(std::make_pair(root1, 1));
        }
        else if (c0 == zero)
        {
            // One double root.
            rmMap.insert(std::make_pair(zero, 2));
        }
        else  // c0 > 0
        {
            // A complex-conjugate pair of roots.
            // Complex z0 = -q1/2 - i*sqrt(c0);
            // Complex z0conj = -q1/2 + i*sqrt(c0);
        }
    }

    void RootsPolynomial::SolveDepressedCubic(const double& c0, const double& c1,
                                              std::map<double, int>& rmMap)
    {
        // Handle the special case of c0 = 0, in which case the polynomial
        // reduces to a depressed quadratic.
        const double zero = 0;
        if (c0 == zero)
        {
            SolveDepressedQuadratic(c1, rmMap);
            auto iter = rmMap.find(zero);
            if (iter != rmMap.end())
            {
                // The quadratic has a root of zero, so the multiplicity must be
                // increased.
                ++iter->second;
            }
            else
            {
                // The quadratic does not have a root of zero.  Insert the one
                // for the cubic.
                rmMap.insert(std::make_pair(zero, 1));
            }
            return;
        }

        // Handle the special case of c0 != 0 and c1 = 0.
        const double oneThird = 1.0 / 3.0;
        if (c1 == zero)
        {
            // One simple real root.
            double root0;
            if (c0 > zero)
            {
                root0 = (double)-std::pow((double)c0, oneThird);
            }
            else
            {
                root0 = (double)std::pow(-(double)c0, oneThird);
            }
            rmMap.insert(std::make_pair(root0, 1));

            // One complex conjugate pair.
            // Complex z0 = root0*(-1 - i*sqrt(3))/2;
            // Complex z0conj = root0*(-1 + i*sqrt(3))/2;
            return;
        }

        // At this time, c0 != 0 and c1 != 0.
        const double rat2 = 2, rat3 = 3, rat4 = 4, rat27 = 27, rat108 = 108;
        double delta = -(rat4 * c1 * c1 * c1 + rat27 * c0 * c0);
        if (delta > zero)
        {
            // Three simple roots.
            double deltaDiv108 = delta / rat108;
            double betaRe = -c0 / rat2;
            double betaIm = std::sqrt(deltaDiv108);
            double theta = std::atan2(betaIm, betaRe);
            double thetaDiv3 = theta / rat3;
            double angle = (double)thetaDiv3;
            double cs = (double)std::cos(angle);
            double sn = (double)std::sin(angle);
            double rhoSqr = betaRe * betaRe + betaIm * betaIm;
            double rhoPowThird = (double)std::pow((double)rhoSqr, 1.0 / 6.0);
            double temp0 = rhoPowThird * cs;
            double temp1 = rhoPowThird * sn * (double)std::sqrt(3.0);
            double root0 = rat2 * temp0;
            double root1 = -temp0 - temp1;
            double root2 = -temp0 + temp1;
            rmMap.insert(std::make_pair(root0, 1));
            rmMap.insert(std::make_pair(root1, 1));
            rmMap.insert(std::make_pair(root2, 1));
        }
        else if (delta < zero)
        {
            // One simple root.
            double deltaDiv108 = delta / rat108;
            double temp0 = -c0 / rat2;
            double temp1 = (double)std::sqrt(-(double)deltaDiv108);
            double temp2 = temp0 - temp1;
            double temp3 = temp0 + temp1;
            if (temp2 >= zero)
            {
                temp2 = (double)std::pow((double)temp2, oneThird);
            }
            else
            {
                temp2 = (double)-std::pow(-(double)temp2, oneThird);
            }
            if (temp3 >= zero)
            {
                temp3 = (double)std::pow((double)temp3, oneThird);
            }
            else
            {
                temp3 = (double)-std::pow(-(double)temp3, oneThird);
            }
            double root0 = temp2 + temp3;
            rmMap.insert(std::make_pair(root0, 1));

            // One complex conjugate pair.
            // Complex z0 = (-root0 - i*sqrt(3*root0*root0+4*c1))/2;
            // Complex z0conj = (-root0 + i*sqrt(3*root0*root0+4*c1))/2;
        }
        else  // delta = 0
        {
            // One simple root and one double root.
            double root0 = -rat3 * c0 / (rat2 * c1);
            double root1 = -rat2 * root0;
            rmMap.insert(std::make_pair(root0, 2));
            rmMap.insert(std::make_pair(root1, 1));
        }
    }

    void RootsPolynomial::SolveBiquadratic(const double& c0, const double& c2,
                                           std::map<double, int>& rmMap)
    {
        // Solve 0 = x^4 + c2*x^2 + c0 = (x^2 + c2/2)^2 + a1, where
        // a1 = c0 - c2^2/2.  We know that c0 != 0 at the time of the function
        // call, so x = 0 is not a root.  The condition c1 = 0 implies the quartic
        // Delta = 256*c0*a1^2.

        double const zero = 0, rat2 = 2, rat256 = 256;
        double c2Half = c2 / rat2;
        double a1 = c0 - c2Half * c2Half;
        double delta = rat256 * c0 * a1 * a1;
        if (delta > zero)
        {
            if (c2 < zero)
            {
                if (a1 < zero)
                {
                    // Four simple roots.
                    double temp0 = (double)std::sqrt(-(double)a1);
                    double temp1 = -c2Half - temp0;
                    double temp2 = -c2Half + temp0;
                    double root1 = (double)std::sqrt((double)temp1);
                    double root0 = -root1;
                    double root2 = (double)std::sqrt((double)temp2);
                    double root3 = -root2;
                    rmMap.insert(std::make_pair(root0, 1));
                    rmMap.insert(std::make_pair(root1, 1));
                    rmMap.insert(std::make_pair(root2, 1));
                    rmMap.insert(std::make_pair(root3, 1));
                }
                else  // a1 > 0
                {
                    // Two simple complex conjugate pairs.
                    // double thetaDiv2 = atan2(sqrt(a1), -c2/2) / 2.0;
                    // double cs = cos(thetaDiv2), sn = sin(thetaDiv2);
                    // double length = pow(c0, 0.25);
                    // Complex z0 = length*(cs + i*sn);
                    // Complex z0conj = length*(cs - i*sn);
                    // Complex z1 = length*(-cs + i*sn);
                    // Complex z1conj = length*(-cs - i*sn);
                }
            }
            else  // c2 >= 0
            {
                // Two simple complex conjugate pairs.
                // Complex z0 = -i*sqrt(c2/2 - sqrt(-a1));
                // Complex z0conj = +i*sqrt(c2/2 - sqrt(-a1));
                // Complex z1 = -i*sqrt(c2/2 + sqrt(-a1));
                // Complex z1conj = +i*sqrt(c2/2 + sqrt(-a1));
            }
        }
        else if (delta < zero)
        {
            // Two simple real roots.
            double temp0 = (double)std::sqrt(-(double)a1);
            double temp1 = -c2Half + temp0;
            double root1 = (double)std::sqrt((double)temp1);
            double root0 = -root1;
            rmMap.insert(std::make_pair(root0, 1));
            rmMap.insert(std::make_pair(root1, 1));

            // One complex conjugate pair.
            // Complex z0 = -i*sqrt(c2/2 + sqrt(-a1));
            // Complex z0conj = +i*sqrt(c2/2 + sqrt(-a1));
        }
        else  // delta = 0
        {
            if (c2 < zero)
            {
                // Two double real roots.
                double root1 = (double)std::sqrt(-(double)c2Half);
                double root0 = -root1;
                rmMap.insert(std::make_pair(root0, 2));
                rmMap.insert(std::make_pair(root1, 2));
            }
            else  // c2 > 0
            {
                // Two double complex conjugate pairs.
                // Complex z0 = -i*sqrt(c2/2);  // multiplicity 2
                // Complex z0conj = +i*sqrt(c2/2);  // multiplicity 2
            }
        }
    }

    void RootsPolynomial::SolveDepressedQuartic(const double& c0, const double& c1,
                                                const double& c2, std::map<double, int>& rmMap)
    {
        // Handle the special case of c0 = 0, in which case the polynomial
        // reduces to a depressed cubic.
        const double zero = 0;
        if (c0 == zero)
        {
            SolveDepressedCubic(c1, c2, rmMap);
            auto iter = rmMap.find(zero);
            if (iter != rmMap.end())
            {
                // The cubic has a root of zero, so the multiplicity must be
                // increased.
                ++iter->second;
            }
            else
            {
                // The cubic does not have a root of zero.  Insert the one
                // for the quartic.
                rmMap.insert(std::make_pair(zero, 1));
            }
            return;
        }

        // Handle the special case of c1 = 0, in which case the quartic is a
        // biquadratic x^4 + c1*x^2 + c0 = (x^2 + c2/2)^2 + (c0 - c2^2/4).
        if (c1 == zero)
        {
            SolveBiquadratic(c0, c2, rmMap);
            return;
        }

        // At this time, c0 != 0 and c1 != 0, which is a requirement for the
        // general solver that must use a root of a special cubic polynomial.
        double const rat2 = 2, rat4 = 4, rat8 = 8, rat12 = 12, rat16 = 16;
        double const rat27 = 27, rat36 = 36;
        double c0sqr = c0 * c0, c1sqr = c1 * c1, c2sqr = c2 * c2;
        double delta = c1sqr * (-rat27 * c1sqr + rat4 * c2 *
            (rat36 * c0 - c2sqr)) + rat16 * c0 * (c2sqr * (c2sqr - rat8 * c0) +
                                                  rat16 * c0sqr);
        double a0 = rat12 * c0 + c2sqr;
        double a1 = rat4 * c0 - c2sqr;

        if (delta > zero)
        {
            if (c2 < zero && a1 < zero)
            {
                // Four simple real roots.
                std::map<double, int> rmCubicMap;
                SolveCubic(c1sqr - rat4 * c0 * c2, rat8 * c0, rat4 * c2, -rat8,
                           rmCubicMap);
                double t = (double)rmCubicMap.rbegin()->first;
                double alphaSqr = rat2 * t - c2;
                double alpha = (double)std::sqrt((double)alphaSqr);
                double sgnC1;
                if (c1 > zero)
                {
                    sgnC1 = 1.0;
                }
                else
                {
                    sgnC1 = -1.0;
                }
                double arg = t * t - c0;
                double beta = (double)(sgnC1 * std::sqrt(std::max((double)arg, 0.0)));
                double D0 = alphaSqr - rat4 * (t + beta);
                double sqrtD0 = (double)std::sqrt(std::max((double)D0, 0.0));
                double D1 = alphaSqr - rat4 * (t - beta);
                double sqrtD1 = (double)std::sqrt(std::max((double)D1, 0.0));
                double root0 = (alpha - sqrtD0) / rat2;
                double root1 = (alpha + sqrtD0) / rat2;
                double root2 = (-alpha - sqrtD1) / rat2;
                double root3 = (-alpha + sqrtD1) / rat2;
                rmMap.insert(std::make_pair(root0, 1));
                rmMap.insert(std::make_pair(root1, 1));
                rmMap.insert(std::make_pair(root2, 1));
                rmMap.insert(std::make_pair(root3, 1));
            }
            else // c2 >= 0 or a1 >= 0
            {
                // Two complex-conjugate pairs.  The values alpha, D0, and D1 are
                // those of the if-block.
                // Complex z0 = (alpha - i*sqrt(-D0))/2;
                // Complex z0conj = (alpha + i*sqrt(-D0))/2;
                // Complex z1 = (-alpha - i*sqrt(-D1))/2;
                // Complex z1conj = (-alpha + i*sqrt(-D1))/2;
            }
        }
        else if (delta < zero)
        {
            // Two simple real roots, one complex-conjugate pair.
            std::map<double, int> rmCubicMap;
            SolveCubic(c1sqr - rat4 * c0 * c2, rat8 * c0, rat4 * c2, -rat8,
                       rmCubicMap);
            double t = (double)rmCubicMap.rbegin()->first;
            double alphaSqr = rat2 * t - c2;
            double alpha = (double)std::sqrt(std::max((double)alphaSqr, 0.0));
            double sgnC1;
            if (c1 > zero)
            {
                sgnC1 = 1.0;  // Leads to BLOCK(18)
            }
            else
            {
                sgnC1 = -1.0;  // Leads to BLOCK(19)
            }
            double arg = t * t - c0;
            double beta = (double)(sgnC1 * std::sqrt(std::max((double)arg, 0.0)));
            double root0, root1;
            if (sgnC1 > 0.0)
            {
                double D1 = alphaSqr - rat4 * (t - beta);
                double sqrtD1 = (double)std::sqrt(std::max((double)D1, 0.0));
                root0 = (-alpha - sqrtD1) / rat2;
                root1 = (-alpha + sqrtD1) / rat2;

                // One complex conjugate pair.
                // Complex z0 = (alpha - i*sqrt(-D0))/2;
                // Complex z0conj = (alpha + i*sqrt(-D0))/2;
            }
            else
            {
                double D0 = alphaSqr - rat4 * (t + beta);
                double sqrtD0 = (double)std::sqrt(std::max((double)D0, 0.0));
                root0 = (alpha - sqrtD0) / rat2;
                root1 = (alpha + sqrtD0) / rat2;

                // One complex conjugate pair.
                // Complex z0 = (-alpha - i*sqrt(-D1))/2;
                // Complex z0conj = (-alpha + i*sqrt(-D1))/2;
            }
            rmMap.insert(std::make_pair(root0, 1));
            rmMap.insert(std::make_pair(root1, 1));
        }
        else  // delta = 0
        {
            if (a1 > zero || (c2 > zero && (a1 != zero || c1 != zero)))
            {
                // One double real root, one complex-conjugate pair.
                double const rat9 = 9;
                double root0 = -c1 * a0 / (rat9 * c1sqr - rat2 * c2 * a1);
                rmMap.insert(std::make_pair(root0, 2));

                // One complex conjugate pair.
                // Complex z0 = -root0 - i*sqrt(c2 + root0^2);
                // Complex z0conj = -root0 + i*sqrt(c2 + root0^2);
            }
            else
            {
                double const rat3 = 3;
                if (a0 != zero)
                {
                    // One double real root, two simple real roots.
                    double const rat9 = 9;
                    double root0 = -c1 * a0 / (rat9 * c1sqr - rat2 * c2 * a1);
                    double alpha = rat2 * root0;
                    double beta = c2 + rat3 * root0 * root0;
                    double discr = alpha * alpha - rat4 * beta;
                    double temp1 = (double)std::sqrt((double)discr);
                    double root1 = (-alpha - temp1) / rat2;
                    double root2 = (-alpha + temp1) / rat2;
                    rmMap.insert(std::make_pair(root0, 2));
                    rmMap.insert(std::make_pair(root1, 1));
                    rmMap.insert(std::make_pair(root2, 1));
                }
                else
                {
                    // One triple real root, one simple real root.
                    double root0 = -rat3 * c1 / (rat4 * c2);
                    double root1 = -rat3 * root0;
                    rmMap.insert(std::make_pair(root0, 3));
                    rmMap.insert(std::make_pair(root1, 1));
                }
            }
        }
    }

    // Support for the GetNumRoots* functions.
    void RootsPolynomial::GetRootInfoDepressedQuadratic(const double& c0,
                                                        std::vector<int>& info)
    {
        double const zero = 0;
        if (c0 < zero)
        {
            // Two simple roots.
            info.push_back(1);
            info.push_back(1);
        }
        else if (c0 == zero)
        {
            // One double root.
            info.push_back(2);  // root is zero
        }
        else  // c0 > 0
        {
            // A complex-conjugate pair of roots.
        }
    }

    void RootsPolynomial::GetRootInfoDepressedCubic(const double& c0,
                                                    const double& c1, std::vector<int>& info)
    {
        // Handle the special case of c0 = 0, in which case the polynomial
        // reduces to a depressed quadratic.
        double const zero = 0;
        if (c0 == zero)
        {
            if (c1 == zero)
            {
                info.push_back(3);  // triple root of zero
            }
            else
            {
                info.push_back(1);  // simple root of zero
                GetRootInfoDepressedQuadratic(c1, info);
            }
            return;
        }

        double const rat4 = 4, rat27 = 27;
        double delta = -(rat4 * c1 * c1 * c1 + rat27 * c0 * c0);
        if (delta > zero)
        {
            // Three simple real roots.
            info.push_back(1);
            info.push_back(1);
            info.push_back(1);
        }
        else if (delta < zero)
        {
            // One simple real root.
            info.push_back(1);
        }
        else  // delta = 0
        {
            // One simple real root and one double real root.
            info.push_back(1);
            info.push_back(2);
        }
    }

    void RootsPolynomial::GetRootInfoBiquadratic(const double& c0,
                                                 const double& c2, std::vector<int>& info)
    {
        // Solve 0 = x^4 + c2*x^2 + c0 = (x^2 + c2/2)^2 + a1, where
        // a1 = c0 - c2^2/2.  We know that c0 != 0 at the time of the function
        // call, so x = 0 is not a root.  The condition c1 = 0 implies the quartic
        // Delta = 256*c0*a1^2.

        double const zero = 0, rat2 = 2, rat256 = 256;
        double c2Half = c2 / rat2;
        double a1 = c0 - c2Half * c2Half;
        double delta = rat256 * c0 * a1 * a1;
        if (delta > zero)
        {
            if (c2 < zero)
            {
                if (a1 < zero)
                {
                    // Four simple roots.
                    info.push_back(1);
                    info.push_back(1);
                    info.push_back(1);
                    info.push_back(1);
                }
                else  // a1 > 0
                {
                    // Two simple complex conjugate pairs.
                }
            }
            else  // c2 >= 0
            {
                // Two simple complex conjugate pairs.
            }
        }
        else if (delta < zero)
        {
            // Two simple real roots, one complex conjugate pair.
            info.push_back(1);
            info.push_back(1);
        }
        else  // delta = 0
        {
            if (c2 < zero)
            {
                // Two double real roots.
                info.push_back(2);
                info.push_back(2);
            }
            else  // c2 > 0
            {
                // Two double complex conjugate pairs.
            }
        }
    }

    void RootsPolynomial::GetRootInfoDepressedQuartic(const double& c0,
                                                      const double& c1, const double& c2, std::vector<int>& info)
    {
        // Handle the special case of c0 = 0, in which case the polynomial
        // reduces to a depressed cubic.
        double const zero = 0;
        if (c0 == zero)
        {
            if (c1 == zero)
            {
                if (c2 == zero)
                {
                    info.push_back(4);  // quadruple root of zero
                }
                else
                {
                    info.push_back(2);  // double root of zero
                    GetRootInfoDepressedQuadratic(c2, info);
                }
            }
            else
            {
                info.push_back(1);  // simple root of zero
                GetRootInfoDepressedCubic(c1, c2, info);
            }
            return;
        }

        // Handle the special case of c1 = 0, in which case the quartic is a
        // biquadratic x^4 + c1*x^2 + c0 = (x^2 + c2/2)^2 + (c0 - c2^2/4).
        if (c1 == zero)
        {
            GetRootInfoBiquadratic(c0, c2, info);
            return;
        }

        // At this time, c0 != 0 and c1 != 0, which is a requirement for the
        // general solver that must use a root of a special cubic polynomial.
        double const rat4 = 4, rat8 = 8, rat12 = 12, rat16 = 16;
        double const rat27 = 27, rat36 = 36;
        double c0sqr = c0 * c0, c1sqr = c1 * c1, c2sqr = c2 * c2;
        double delta = c1sqr * (-rat27 * c1sqr + rat4 * c2 *
            (rat36 * c0 - c2sqr)) + rat16 * c0 * (c2sqr * (c2sqr - rat8 * c0) +
                                                  rat16 * c0sqr);
        double a0 = rat12 * c0 + c2sqr;
        double a1 = rat4 * c0 - c2sqr;

        if (delta > zero)
        {
            if (c2 < zero && a1 < zero)
            {
                // Four simple real roots.
                info.push_back(1);
                info.push_back(1);
                info.push_back(1);
                info.push_back(1);
            }
            else // c2 >= 0 or a1 >= 0
            {
                // Two complex-conjugate pairs.
            }
        }
        else if (delta < zero)
        {
            // Two simple real roots, one complex-conjugate pair.
            info.push_back(1);
            info.push_back(1);
        }
        else  // delta = 0
        {
            if (a1 > zero || (c2 > zero && (a1 != zero || c1 != zero)))
            {
                // One double real root, one complex-conjugate pair.
                info.push_back(2);
            }
            else
            {
                if (a0 != zero)
                {
                    // One double real root, two simple real roots.
                    info.push_back(2);
                    info.push_back(1);
                    info.push_back(1);
                }
                else
                {
                    // One triple real root, one simple real root.
                    info.push_back(3);
                    info.push_back(1);
                }
            }
        }
    }

    // Support for the Find functions.
    int RootsPolynomial::FindRecursive(int degree, const double* c, double tmin, double tmax,
                                       unsigned int maxIterations, double* roots)
    {
        // The base of the recursion.
        double const zero = 0.0;
        double root = zero;
        if (degree == 1)
        {
            int numRoots;
            if (c[1] != zero)
            {
                root = -c[0] / c[1];
                numRoots = 1;
            }
            else if (c[0] == zero)
            {
                root = zero;
                numRoots = 1;
            }
            else
            {
                numRoots = 0;
            }

            if (numRoots > 0 && tmin <= root && root <= tmax)
            {
                roots[0] = root;
                return 1;
            }
            return 0;
        }

        // Find the roots of the derivative polynomial scaled by 1/degree.  The
        // scaling avoids the factorial growth in the coefficients; for example,
        // without the scaling, the high-order term x^d becomes (d!)*x through
        // multiple differentiations.  With the scaling we instead get x.  This
        // leads to better numerical behavior of the root finder.
        int derivDegree = degree - 1;
        std::vector<double> derivCoeff(derivDegree + 1);
        std::vector<double> derivRoots(derivDegree);
        for (int i = 0; i <= derivDegree; ++i)
        {
            derivCoeff[i] = c[i + 1] * (double)(i + 1) / (double)degree;
        }
        int numDerivRoots = FindRecursive(degree - 1, &derivCoeff[0], tmin, tmax,
                                          maxIterations, &derivRoots[0]);

        int numRoots = 0;
        if (numDerivRoots > 0)
        {
            // Find root on [tmin,derivRoots[0]].
            if (Find(degree, c, tmin, derivRoots[0], maxIterations, root))
            {
                roots[numRoots++] = root;
            }

            // Find root on [derivRoots[i],derivRoots[i+1]].
            for (int i = 0; i <= numDerivRoots - 2; ++i)
            {
                if (Find(degree, c, derivRoots[i], derivRoots[i + 1],
                         maxIterations, root))
                {
                    roots[numRoots++] = root;
                }
            }

            // Find root on [derivRoots[numDerivRoots-1],tmax].
            if (Find(degree, c, derivRoots[numDerivRoots - 1], tmax,
                     maxIterations, root))
            {
                roots[numRoots++] = root;
            }
        }
        else
        {
            // The polynomial is monotone on [tmin,tmax], so has at most one root.
            if (Find(degree, c, tmin, tmax, maxIterations, root))
            {
                roots[numRoots++] = root;
            }
        }
        return numRoots;
    }

    double RootsPolynomial::Evaluate(int degree, const double* c, double t)
    {
        int i = degree;
        double result = c[i];
        while (--i >= 0)
        {
            result = t * result + c[i];
        }
        return result;
    }





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

    void RootsPolynomial::SolveCubic(const double& p0, const double& p1, const double& p2, const double& p3,
                                     std::map<double, int>& rmMap)
    {
        double const rat2 = 2, rat3 = 3;
        double q0 = p0 / p3;
        double q1 = p1 / p3;
        double q2 = p2 / p3;
        double q2third = q2 / rat3;
        double c0 = q0 - q2third * (q1 - rat2 * q2third * q2third);
        double c1 = q1 - q2 * q2third;

        std::map<double, int> rmLocalMap;
        SolveDepressedCubic(c0, c1, rmLocalMap);

        rmMap.clear();
        for (auto& rm : rmLocalMap)
        {
            double root = rm.first - q2third;
            rmMap.insert(std::make_pair(root, rm.second));
        }
    }

    void RootsPolynomial::SolveQuartic(const double& p0, const double& p1, const double& p2, const double& p3, const double& p4,
                                       std::map<double, int>& rmMap)
    {
        double const rat2 = 2, rat3 = 3, rat4 = 4, rat6 = 6;
        double q0 = p0 / p4;
        double q1 = p1 / p4;
        double q2 = p2 / p4;
        double q3 = p3 / p4;
        double q3fourth = q3 / rat4;
        double q3fourthSqr = q3fourth * q3fourth;
        double c0 = q0 - q3fourth * (q1 - q3fourth * (q2 - q3fourthSqr * rat3));
        double c1 = q1 - rat2 * q3fourth * (q2 - rat4 * q3fourthSqr);
        double c2 = q2 - rat6 * q3fourthSqr;

        std::map<double, int> rmLocalMap;
        SolveDepressedQuartic(c0, c1, c2, rmLocalMap);

        rmMap.clear();
        for (auto& rm : rmLocalMap)
        {
            double root = rm.first - q3fourth;
            rmMap.insert(std::make_pair(root, rm.second));
        }
    }

    void RootsPolynomial::GetRootInfoQuadratic(const double& p0, const double& p1, const double& p2,
                                               std::vector<int>& info)
    {
        double const rat2 = 2;
        double q0 = p0 / p2;
        double q1 = p1 / p2;
        double q1half = q1 / rat2;
        double c0 = q0 - q1half * q1half;

        info.clear();
        info.reserve(2);
        GetRootInfoDepressedQuadratic(c0, info);
    }

    void RootsPolynomial::GetRootInfoCubic(const double& p0, const double& p1, const double& p2, const double& p3,
                                           std::vector<int>& info)
    {
        double const rat2 = 2, rat3 = 3;
        double q0 = p0 / p3;
        double q1 = p1 / p3;
        double q2 = p2 / p3;
        double q2third = q2 / rat3;
        double c0 = q0 - q2third * (q1 - rat2 * q2third * q2third);
        double c1 = q1 - q2 * q2third;

        info.clear();
        info.reserve(3);
        GetRootInfoDepressedCubic(c0, c1, info);
    }

    void RootsPolynomial::GetRootInfoQuartic(const double& p0, const double& p1, const double& p2, const double& p3, const double& p4,
                                             std::vector<int>& info)
    {
        double const rat2 = 2, rat3 = 3, rat4 = 4, rat6 = 6;
        double q0 = p0 / p4;
        double q1 = p1 / p4;
        double q2 = p2 / p4;
        double q3 = p3 / p4;
        double q3fourth = q3 / rat4;
        double q3fourthSqr = q3fourth * q3fourth;
        double c0 = q0 - q3fourth * (q1 - q3fourth * (q2 - q3fourthSqr * rat3));
        double c1 = q1 - rat2 * q3fourth * (q2 - rat4 * q3fourthSqr);
        double c2 = q2 - rat6 * q3fourthSqr;

        info.clear();
        info.reserve(4);
        GetRootInfoDepressedQuartic(c0, c1, c2, info);
    }

    int RootsPolynomial::Find(int degree, const double* c, unsigned int maxIterations, double* roots)
    {
        if (degree >= 0 && c)
        {
            double const zero = 0.0;
            while (degree >= 0 && c[degree] == zero)
            {
                --degree;
            }

            if (degree > 0)
            {
                // Compute the Cauchy bound.
                double const one = 1.0;
                double invLeading = one / c[degree];
                double maxValue = zero;
                for (int i = 0; i < degree; ++i)
                {
                    double value = std::abs(c[i] * invLeading);
                    if (value > maxValue)
                    {
                        maxValue = value;
                    }
                }
                double bound = one + maxValue;

                return FindRecursive(degree, c, -bound, bound, maxIterations,
                                     roots);
            }
            else if (degree == 0)
            {
                // The polynomial is a nonzero constant.
                return 0;
            }
            else
            {
                // The polynomial is identically zero.
                roots[0] = zero;
                return 1;
            }
        }
        else
        {
            // Invalid degree or c.
            return 0;
        }
    }

    bool RootsPolynomial::Find(int degree, const double* c, double tmin, double tmax,
                               unsigned int maxIterations, double& root)
    {
        double const zero = 0.0;
        double pmin = Evaluate(degree, c, tmin);
        if (pmin == zero)
        {
            root = tmin;
            return true;
        }
        double pmax = Evaluate(degree, c, tmax);
        if (pmax == zero)
        {
            root = tmax;
            return true;
        }

        if (pmin * pmax > zero)
        {
            // It is not known whether the interval bounds a root.
            return false;
        }

        if (tmin >= tmax)
        {
            // Invalid ordering of interval endpoitns. 
            return false;
        }

        for (unsigned int i = 1; i <= maxIterations; ++i)
        {
            root = (0.5) * (tmin + tmax);

            // This test is designed for 'float' or 'double' when tmin and tmax
            // are consecutive floating-point numbers.
            if (root == tmin || root == tmax)
            {
                break;
            }

            double p = Evaluate(degree, c, root);
            double product = p * pmin;
            if (product < zero)
            {
                tmax = root;
                pmax = p;
            }
            else if (product > zero)
            {
                tmin = root;
                pmin = p;
            }
            else
            {
                break;
            }
        }

        return true;
    }
}