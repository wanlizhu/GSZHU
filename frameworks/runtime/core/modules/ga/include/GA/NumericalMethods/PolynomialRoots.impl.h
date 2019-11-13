#pragma once

#include "PolynomialRoots.h"

namespace GA
{
    namespace internal
    {
        template<typename T>
        static void SolveDepressedQuadratic(const T& c0, std::map<T, int>* rmMap);
    
        template<typename T>
        static void SolveDepressedCubic(const T& c0, const T& c1, std::map<T, int>* rmMap);

        template<typename T>
        static void SolveDepressedQuartic(const T& c0, const T& c1, const T& c2, std::map<T, int>* rmMap);

        template<typename T>
        static void SolveBiquadratic(const T& c0, const T& c2, std::map<T, int>* rmMap);

        template<typename T>
        static int GetNumRootsDepressedQuadratic(const T& c0, std::vector<int>* rmList);

        template<typename T>
        static int GetNumRootsDepressedCubic(const T& c0, const T& c1, std::vector<int>* rmList);

        template<typename T>
        static int GetNumRootsDepressedQuartic(const T& c0, const T& c1, const T& c2, std::vector<int>* rmList);
    
        template<typename T>
        static int GetNumRootsBiquadratic(const T& c0, const T& c2, std::vector<int>* rmList);
    



        template<typename T>
        static int FindRootsRecursive(int degree, 
                                      const T* c,  
                                      T tmin,
                                      T tmax,
                                      int maxIterations,
                                      T* roots);

        template<typename T>
        static T Evaluate(int degree, const T* c, T t);
    } // namespace internal

    template<typename T>
    int PolynomialRoots<T>::FindRoots(int degree,
                                      const Scalar* coeffs,
                                      int maxIterations, 
                                      Scalar* roots)
    {
        if (degree >= 0 && c)
        {
            const zero = (T)0;
            while (degree >= 0 && c[degree] == zero)
            {
                --degree;
            }

            if (degree > 0)
            {
                // Compute the Cauchy bound.
                const one = (T)1;
                T invLeading = one / c[degree];
                T maxValue = zero;
                for (int i = 0; i < degree; ++i)
                {
                    T value = std::abs(c[i] * invLeading);
                    if (value > maxValue)
                    {
                        maxValue = value;
                    }
                }
                T bound = one + maxValue;

                return internal::FindRootsRecursive(degree, 
                                                    c,
                                                    -bound, 
                                                    bound, 
                                                    maxIterations,
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

    template<typename T>
    bool PolynomialRoots<T>::FindRootInside(int degree,
                                            const Scalar* coeffs,
                                            Scalar tmin, 
                                            Scalar tmax,
                                            int maxIterations, 
                                            Scalar* root)
    {
        const zero = (T)0;
        T pmin = internal::Evaluate(degree, c, tmin);
        if (pmin == zero)
        {
            root = tmin;
            return true;
        }
        T pmax = internal::Evaluate(degree, c, tmax);
        if (pmax == zero)
        {
            root = tmax;
            return true;
        }

        if (pmin*pmax > zero)
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
            root = ((T)0.5) * (tmin + tmax);

            // This test is designed for 'float' or 'double' when tmin and tmax
            // are consecutive floating-point numbers.
            if (root == tmin || root == tmax)
            {
                break;
            }

            T p = internal::Evaluate(degree, c, root);
            T product = p * pmin;
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

    template<typename T>
    void PolynomialRoots<T>::SolveQuadratic(const Scalar& p0, 
                                            const Scalar& p1,
                                            const Scalar& p2, 
                                            std::map<Scalar, int>* rmMap)
    {
        Scalar const rat2 = 2;
        Scalar q0 = p0 / p2;
        Scalar q1 = p1 / p2;
        Scalar q1half = q1 / rat2;
        Scalar c0 = q0 - q1half * q1half;

        std::map<Scalar, int> rmLocalMap;
        internal::SolveDepressedQuadratic(c0, &rmLocalMap);

        rmMap->clear();
        for (auto& rm : rmLocalMap)
        {
            Scalar root = rm.first - q1half;
            rmMap->insert(std::make_pair(root, rm.second));
        }
    }

    template<typename T>
    void PolynomialRoots<T>::SolveCubic(const Scalar& p0,
                                        const Scalar& p1,
                                        const Scalar& p2,
                                        const Scalar& p3,
                                        std::map<Scalar, int>* rmMap)
    {
        Scalar const rat2 = 2, rat3 = 3;
        Scalar q0 = p0 / p3;
        Scalar q1 = p1 / p3;
        Scalar q2 = p2 / p3;
        Scalar q2third = q2 / rat3;
        Scalar c0 = q0 - q2third * (q1 - rat2 * q2third * q2third);
        Scalar c1 = q1 - q2 * q2third;

        std::map<Scalar, int> rmLocalMap;
        internal::SolveDepressedCubic(c0, c1, &rmLocalMap);

        rmMap->clear();
        for (auto& rm : rmLocalMap)
        {
            Scalar root = rm.first - q2third;
            rmMap->insert(std::make_pair(root, rm.second));
        }
    }

    template<typename T>
    void PolynomialRoots<T>::SolveQuartic(const Scalar& p0,
                                          const Scalar& p1,
                                          const Scalar& p2,
                                          const Scalar& p3,
                                          const Scalar& p4,
                                          std::map<Scalar, int>* rmMap)
    {
        Scalar const rat2 = 2, rat3 = 3, rat4 = 4, rat6 = 6;
        Scalar q0 = p0 / p4;
        Scalar q1 = p1 / p4;
        Scalar q2 = p2 / p4;
        Scalar q3 = p3 / p4;
        Scalar q3fourth = q3 / rat4;
        Scalar q3fourthSqr = q3fourth *  q3fourth;
        Scalar c0 = q0 - q3fourth * (q1 - q3fourth * (q2 - q3fourthSqr * rat3));
        Scalar c1 = q1 - rat2 * q3fourth * (q2 - rat4 * q3fourthSqr);
        Scalar c2 = q2 - rat6 * q3fourthSqr;

        std::map<Scalar, int> rmLocalMap;
        internal::SolveDepressedQuartic(c0, c1, c2, &rmLocalMap);

        rmMap->clear();
        for (auto& rm : rmLocalMap)
        {
            Scalar root = rm.first - q3fourth;
            rmMap->insert(std::make_pair(root, rm.second));
        }
    }

    template<typename T>
    int PolynomialRoots<T>::GetNumRootsQuadratic(const Scalar& p0, 
                                                 const Scalar& p1,
                                                 const Scalar& p2, 
                                                 std::vector<int>* rmList)
    {
        Scalar const rat2 = 2;
        Scalar q0 = p0 / p2;
        Scalar q1 = p1 / p2;
        Scalar q1half = q1 / rat2;
        Scalar c0 = q0 - q1half * q1half;

        if (rmList)
        {
            rmList->clear();
            rmList->reserve(2);
        }
        
        return internal::GetNumRootsDepressedQuadratic(c0, rmList);
    }

    template<typename T>
    int PolynomialRoots<T>::GetNumRootsCubic(const Scalar& p0, 
                                             const Scalar& p1,
                                             const Scalar& p2, 
                                             const Scalar& p3, 
                                             std::vector<int>* rmList)
    {
        Scalar const rat2 = 2, rat3 = 3;
        Scalar q0 = p0 / p3;
        Scalar q1 = p1 / p3;
        Scalar q2 = p2 / p3;
        Scalar q2third = q2 / rat3;
        Scalar c0 = q0 - q2third * (q1 - rat2 * q2third * q2third);
        Scalar c1 = q1 - q2 * q2third;

        if (rmList)
        {
            rmList->clear();
            rmList->reserve(3);
        }
        return internal::GetDiscriminantDepressedCubic(c0, c1, rmList);
    }

    template<typename T>
    int PolynomialRoots<T>::GetNumRootsQuartic(const Scalar& p0, 
                                               const Scalar& p1,
                                               const Scalar& p2, 
                                               const Scalar& p3, 
                                               const Scalar& p4, 
                                               std::vector<int>* rmList)
    {
        Scalar const rat2 = 2, rat3 = 3, rat4 = 4, rat6 = 6;
        Scalar q0 = p0 / p4;
        Scalar q1 = p1 / p4;
        Scalar q2 = p2 / p4;
        Scalar q3 = p3 / p4;
        Scalar q3fourth = q3 / rat4;
        Scalar q3fourthSqr = q3fourth *  q3fourth;
        Scalar c0 = q0 - q3fourth * (q1 - q3fourth * (q2 - q3fourthSqr * rat3));
        Scalar c1 = q1 - rat2 * q3fourth * (q2 - rat4 * q3fourthSqr);
        Scalar c2 = q2 - rat6 * q3fourthSqr;

        if (rmList)
        {
            rmList->clear();
            rmList->reserve(4);
        }
        
        return internal::GetNumRootsDepressedQuartic(c0, c1, c2, rmList);
    }



    namespace internal
    {
        template<typename T>
        static void SolveDepressedQuadratic(const T& c0, std::map<T, int>* rmMap)
        {
            const T zero = 0;
            if (c0 < zero)
            {
                // Two simple roots.
                T root1 = (T)std::sqrt(-(double)c0);
                T root0 = -root1;
                rmMap->insert(std::make_pair(root0, 1));
                rmMap->insert(std::make_pair(root1, 1));
            }
            else if (c0 == zero)
            {
                // One double root.
                rmMap->insert(std::make_pair(zero, 2));
            }
            else  // c0 > 0
            {
                // A complex-conjugate pair of roots.
                // Complex z0 = -q1/2 - i*sqrt(c0);
                // Complex z0conj = -q1/2 + i*sqrt(c0);
            }
        }
    
        template<typename T>
        static void SolveDepressedCubic(const T& c0, const T& c1, std::map<T, int>* rmMap)
        {
            // Handle the special case of c0 = 0, in which case the polynomial
            // reduces to a depressed quadratic.
            const T zero = 0;
            if (c0 == zero)
            {
                internal::SolveDepressedQuadratic(c1, rmMap);
                auto iter = rmMap->find(zero);
                if (iter != rmMap->end())
                {
                    // The quadratic has a root of zero, 
                    // so the multiplicity must be increased.
                    ++iter->second;
                }
                else
                {
                    // The quadratic does not have a root of zero.  
                    // Insert the one for the cubic.
                    rmMap->insert(std::make_pair(zero, 1));
                }
                return;
            }

            // Handle the special case of c0 != 0 and c1 = 0.
            const double oneThird = 1.0 / 3.0;
            if (c1 == zero)
            {
                // One simple real root.
                T root0;
                if (c0 > zero)
                {
                    root0 = (T)-std::pow((double)c0, oneThird);
                }
                else
                {
                    root0 = (T)std::pow(-(double)c0, oneThird);
                }
                rmMap->insert(std::make_pair(root0, 1));

                // One complex conjugate pair.
                // Complex z0 = root0*(-1 - i*sqrt(3))/2;
                // Complex z0conj = root0*(-1 + i*sqrt(3))/2;
                return;
            }

            // At this time, c0 != 0 and c1 != 0.
            const T rat2 = 2, rat3 = 3, rat4 = 4, rat27 = 27, rat108 = 108;
            T delta = -(rat4 * c1 * c1 * c1 + rat27 * c0 * c0);
            if (delta > zero)
            {
                // Three simple roots.
                T deltaDiv108 = delta / rat108;
                T betaRe = -c0 / rat2;
                T betaIm = std::sqrt(deltaDiv108);
                T theta = std::atan2(betaIm, betaRe);
                T thetaDiv3 = theta / rat3;
                double angle = (double)thetaDiv3;
                T cs = (T)std::cos(angle);
                T sn = (T)std::sin(angle);
                T rhoSqr = betaRe * betaRe + betaIm * betaIm;
                T rhoPowThird = (T)std::pow((double)rhoSqr, 1.0 / 6.0);
                T temp0 = rhoPowThird * cs;
                T temp1 = rhoPowThird * sn * (T)std::sqrt(3.0);
                T root0 = rat2 * temp0;
                T root1 = -temp0 - temp1;
                T root2 = -temp0 + temp1;
                rmMap->insert(std::make_pair(root0, 1));
                rmMap->insert(std::make_pair(root1, 1));
                rmMap->insert(std::make_pair(root2, 1));
            }
            else if (delta < zero)
            {
                // One simple root.
                T deltaDiv108 = delta / rat108;
                T temp0 = -c0 / rat2;
                T temp1 = (T)std::sqrt(-(double)deltaDiv108);
                T temp2 = temp0 - temp1;
                T temp3 = temp0 + temp1;
                if (temp2 >= zero)
                {
                    temp2 = (T)std::pow((double)temp2, oneThird);
                }
                else
                {
                    temp2 = (T)-std::pow(-(double)temp2, oneThird);
                }
                if (temp3 >= zero)
                {
                    temp3 = (T)std::pow((double)temp3, oneThird);
                }
                else
                {
                    temp3 = (T)-std::pow(-(double)temp3, oneThird);
                }
                T root0 = temp2 + temp3;
                rmMap->insert(std::make_pair(root0, 1));

                // One complex conjugate pair.
                // Complex z0 = (-root0 - i*sqrt(3*root0*root0+4*c1))/2;
                // Complex z0conj = (-root0 + i*sqrt(3*root0*root0+4*c1))/2;
            }
            else  // delta = 0
            {
                // One simple root and one double root.
                T root0 = -rat3 * c0 / (rat2 * c1);
                T root1 = -rat2 * root0;
                rmMap->insert(std::make_pair(root0, 2));
                rmMap->insert(std::make_pair(root1, 1));
            }
        }

        template<typename T>
        static void SolveDepressedQuartic(const T& c0, const T& c1, const T& c2, std::map<T, int>* rmMap)
        {
            // Handle the special case of c0 = 0, in which case the polynomial
            // reduces to a depressed cubic.
            const T zero = 0;
            if (c0 == zero)
            {
                internal::SolveDepressedCubic(c1, c2, rmMap);
                auto iter = rmMap->find(zero);
                if (iter != rmMap->end())
                {
                    // The cubic has a root of zero, so the multiplicity must be
                    // increased.
                    ++iter->second;
                }
                else
                {
                    // The cubic does not have a root of zero.  Insert the one
                    // for the quartic.
                    rmMap->insert(std::make_pair(zero, 1));
                }
                return;
            }

            // Handle the special case of c1 = 0, in which case the quartic is a
            // biquadratic x^4 + c1*x^2 + c0 = (x^2 + c2/2)^2 + (c0 - c2^2/4).
            if (c1 == zero)
            {
                internal::SolveBiquadratic(c0, c2, rmMap);
                return;
            }

            // At this time, c0 != 0 and c1 != 0, which is a requirement for the
            // general solver that must use a root of a special cubic polynomial.
            const T rat2 = 2, rat4 = 4, rat8 = 8, rat12 = 12, rat16 = 16;
            const T rat27 = 27, rat36 = 36;
            T c0sqr = c0 * c0, c1sqr = c1 * c1, c2sqr = c2 * c2;
            T delta = c1sqr * (-rat27 * c1sqr + rat4 * c2 *
                (rat36 * c0 - c2sqr)) + rat16 * c0 * (c2sqr * (c2sqr - rat8 * c0) +
                rat16 * c0sqr);
            T a0 = rat12 * c0 + c2sqr;
            T a1 = rat4 * c0 - c2sqr;

            if (delta > zero)
            {
                if (c2 < zero && a1 < zero)
                {
                    // Four simple real roots.
                    std::map<T, int> rmCubicMap;
                    PolynomialRoots<T>::SolveCubic(c1sqr - rat4 * c0 * c2, 
                                                   rat8 * c0, 
                                                   rat4 * c2, 
                                                   -rat8,
                                                   &rmCubicMap);
                    T t = (T)rmCubicMap.rbegin()->first;
                    T alphaSqr = rat2 * t - c2;
                    T alpha = (T)std::sqrt((double)alphaSqr);
                    double sgnC1;
                    if (c1 > zero)
                    {
                        sgnC1 = 1.0;
                    }
                    else
                    {
                        sgnC1 = -1.0;
                    }
                    T arg = t * t - c0;
                    T beta = (T)(sgnC1 * std::sqrt(std::max((double)arg, 0.0)));
                    T D0 = alphaSqr - rat4 * (t + beta);
                    T sqrtD0 = (T)std::sqrt(std::max((double)D0, 0.0));
                    T D1 = alphaSqr - rat4 * (t - beta);
                    T sqrtD1 = (T)std::sqrt(std::max((double)D1, 0.0));
                    T root0 = (alpha - sqrtD0) / rat2;
                    T root1 = (alpha + sqrtD0) / rat2;
                    T root2 = (-alpha - sqrtD1) / rat2;
                    T root3 = (-alpha + sqrtD1) / rat2;
                    rmMap->insert(std::make_pair(root0, 1));
                    rmMap->insert(std::make_pair(root1, 1));
                    rmMap->insert(std::make_pair(root2, 1));
                    rmMap->insert(std::make_pair(root3, 1));
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
                std::map<T, int> rmCubicMap;
                PolynomialRoots<T>::SolveCubic(c1sqr - rat4 * c0 * c2, 
                                               rat8 * c0,
                                               rat4 * c2, 
                                               -rat8,
                                               &rmCubicMap);
                T t = (T)rmCubicMap.rbegin()->first;
                T alphaSqr = rat2 * t - c2;
                T alpha = (T)std::sqrt(std::max((double)alphaSqr, 0.0));
                double sgnC1;
                if (c1 > zero)
                {
                    sgnC1 = 1.0;  
                }
                else
                {
                    sgnC1 = -1.0;
                }
                T arg = t * t - c0;
                T beta = (T)(sgnC1 * std::sqrt(std::max((double)arg, 0.0)));
                T root0, root1;
                if (sgnC1 > 0.0)
                {
                    T D1 = alphaSqr - rat4 * (t - beta);
                    T sqrtD1 = (T)std::sqrt(std::max((double)D1, 0.0));
                    root0 = (-alpha - sqrtD1) / rat2;
                    root1 = (-alpha + sqrtD1) / rat2;

                    // One complex conjugate pair.
                    // Complex z0 = (alpha - i*sqrt(-D0))/2;
                    // Complex z0conj = (alpha + i*sqrt(-D0))/2;
                }
                else
                {
                    T D0 = alphaSqr - rat4 * (t + beta);
                    T sqrtD0 = (T)std::sqrt(std::max((double)D0, 0.0));
                    root0 = (alpha - sqrtD0) / rat2;
                    root1 = (alpha + sqrtD0) / rat2;

                    // One complex conjugate pair.
                    // Complex z0 = (-alpha - i*sqrt(-D1))/2;
                    // Complex z0conj = (-alpha + i*sqrt(-D1))/2;
                }
                rmMap->insert(std::make_pair(root0, 1));
                rmMap->insert(std::make_pair(root1, 1));
            }
            else  // delta = 0
            {
                if (a1 > zero || (c2 > zero && (a1 != zero || c1 != zero)))
                {
                    // One double real root, one complex-conjugate pair.
                    const T rat9 = 9;
                    T root0 = -c1 * a0 / (rat9 * c1sqr - rat2 * c2 * a1);
                    rmMap->insert(std::make_pair(root0, 2));

                    // One complex conjugate pair.
                    // Complex z0 = -root0 - i*sqrt(c2 + root0^2);
                    // Complex z0conj = -root0 + i*sqrt(c2 + root0^2);
                }
                else
                {
                    const T rat3 = 3;
                    if (a0 != zero)
                    {
                        // One double real root, two simple real roots.
                        const T rat9 = 9;
                        T root0 = -c1 * a0 / (rat9 * c1sqr - rat2 * c2 * a1);
                        T alpha = rat2 * root0;
                        T beta = c2 + rat3 * root0 * root0;
                        T discr = alpha * alpha - rat4 * beta;
                        T temp1 = (T)std::sqrt((double)discr);
                        T root1 = (-alpha - temp1) / rat2;
                        T root2 = (-alpha + temp1) / rat2;
                        rmMap->insert(std::make_pair(root0, 2));
                        rmMap->insert(std::make_pair(root1, 1));
                        rmMap->insert(std::make_pair(root2, 1));
                    }
                    else
                    {
                        // One triple real root, one simple real root.
                        T root0 = -rat3 * c1 / (rat4 * c2);
                        T root1 = -rat3 * root0;
                        rmMap->insert(std::make_pair(root0, 3));
                        rmMap->insert(std::make_pair(root1, 1));
                    }
                }
            }
        }

        template<typename T>
        static void SolveBiquadratic(const T& c0, const T& c2, std::map<T, int>* rmMap)
        {
            // Solve 0 = x^4 + c2*x^2 + c0 = (x^2 + c2/2)^2 + a1, where
            // a1 = c0 - c2^2/2.  We know that c0 != 0 at the time of the function
            // call, so x = 0 is not a root.  The condition c1 = 0 implies the quartic
            // Delta = 256*c0*a1^2.

            const T zero = 0, rat2 = 2, rat256 = 256;
            T c2Half = c2 / rat2;
            T a1 = c0 - c2Half * c2Half;
            T delta = rat256 * c0 * a1 * a1;
            if (delta > zero)
            {
                if (c2 < zero)
                {
                    if (a1 < zero)
                    {
                        // Four simple roots.
                        T temp0 = (T)std::sqrt(-(double)a1);
                        T temp1 = -c2Half - temp0;
                        T temp2 = -c2Half + temp0;
                        T root1 = (T)std::sqrt((double)temp1);
                        T root0 = -root1;
                        T root2 = (T)std::sqrt((double)temp2);
                        T root3 = -root2;
                        rmMap->insert(std::make_pair(root0, 1));
                        rmMap->insert(std::make_pair(root1, 1));
                        rmMap->insert(std::make_pair(root2, 1));
                        rmMap->insert(std::make_pair(root3, 1));
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
                T temp0 = (T)std::sqrt(-(double)a1);
                T temp1 = -c2Half + temp0;
                T root1 = (T)std::sqrt((double)temp1);
                T root0 = -root1;
                rmMap->insert(std::make_pair(root0, 1));
                rmMap->insert(std::make_pair(root1, 1));

                // One complex conjugate pair.
                // Complex z0 = -i*sqrt(c2/2 + sqrt(-a1));
                // Complex z0conj = +i*sqrt(c2/2 + sqrt(-a1));
            }
            else  // delta = 0
            {
                if (c2 < zero)
                {
                    // Two double real roots.
                    T root1 = (T)std::sqrt(-(double)c2Half);
                    T root0 = -root1;
                    rmMap->insert(std::make_pair(root0, 2));
                    rmMap->insert(std::make_pair(root1, 2));
                }
                else  // c2 > 0
                {
                    // Two double complex conjugate pairs.
                    // Complex z0 = -i*sqrt(c2/2);  // multiplicity 2
                    // Complex z0conj = +i*sqrt(c2/2);  // multiplicity 2
                }
            }
        }

        template<typename T>
        static int GetNumRootsDepressedQuadratic(const T& c0, std::vector<int>* rmList)
        {
            static std::vector<int> _tmp;
            _tmp.clear();
            rmList = rmList == nullptr ? &_tmp : rmList;
    
            const T zero = 0;
            if (c0 < zero)
            {
                // Two simple roots.
                rmList->push_back(1);
                rmList->push_back(1);
            }
            else if (c0 == zero)
            {
                // One double root.
                rmList->push_back(2);  // root is zero
            }
            else  // c0 > 0
            {
                // A complex-conjugate pair of roots.
            }

            return (int)rmList->size();
        }

        template<typename T>
        static int GetNumRootsDepressedCubic(const T& c0, const T& c1, std::vector<int>* rmList)
        {
            static std::vector<int> _tmp;
            _tmp.clear();
            rmList = rmList == nullptr ? &_tmp : rmList;

            // Handle the special case of c0 = 0, in which case the polynomial
            // reduces to a depressed quadratic.
            const T zero = 0;
            if (c0 == zero)
            {
                if (c1 == zero)
                {
                    rmList->push_back(3);  // triple root of zero
                }
                else
                {
                    rmList->push_back(1);  // simple root of zero
                    internal::GetNumRootsDepressedQuadratic(c1, rmList);
                }
                return (int)rmList->size();
            }

            const T rat4 = 4, rat27 = 27;
            T delta = -(rat4 * c1 * c1 * c1 + rat27 * c0 * c0);
            if (delta > zero)
            {
                // Three simple real roots.
                rmList->push_back(1);
                rmList->push_back(1);
                rmList->push_back(1);
            }
            else if (delta < zero)
            {
                // One simple real root.
                rmList->push_back(1);
            }
            else  // delta = 0
            {
                // One simple real root and one double real root.
                rmList->push_back(1);
                rmList->push_back(2);
            }

            return (int)rmList->size();
        }

        template<typename T>
        static int GetNumRootsDepressedQuartic(const T& c0, const T& c1, const T& c2, std::vector<int>* rmList)
        {
            static std::vector<int> _tmp;
            _tmp.clear();
            rmList = rmList == nullptr ? &_tmp : rmList;

            // Handle the special case of c0 = 0, in which case the polynomial
            // reduces to a depressed cubic.
            const T zero = 0;
            if (c0 == zero)
            {
                if (c1 == zero)
                {
                    if (c2 == zero)
                    {
                        rmList->push_back(4);  // quadruple root of zero
                    }
                    else
                    {
                        rmList->push_back(2);  // double root of zero
                        internal::GetNumRootsDepressedQuadratic(c2, rmList);
                    }
                }
                else
                {
                    rmList->push_back(1);  // simple root of zero
                    internal::GetNumRootsDepressedCubic(c1, c2, rmList);
                }
                
                return (int)rmList->size();
            }

            // Handle the special case of c1 = 0, in which case the quartic is a
            // biquadratic x^4 + c1*x^2 + c0 = (x^2 + c2/2)^2 + (c0 - c2^2/4).
            if (c1 == zero)
            {
                internal::GetNumRootsBiquadratic(c0, c2, rmList);
                return (int)rmList->size();
            }

            // At this time, c0 != 0 and c1 != 0, which is a requirement for the
            // general solver that must use a root of a special cubic polynomial.
            const T rat4 = 4, rat8 = 8, rat12 = 12, rat16 = 16;
            const T rat27 = 27, rat36 = 36;
            T c0sqr = c0 * c0, c1sqr = c1 * c1, c2sqr = c2 * c2;
            T delta = c1sqr * (-rat27 * c1sqr + rat4 * c2 * (rat36 * c0 - c2sqr)) 
                    + rat16 * c0 * (c2sqr * (c2sqr - rat8 * c0) + rat16 * c0sqr);
            T a0 = rat12 * c0 + c2sqr;
            T a1 = rat4 * c0 - c2sqr;

            if (delta > zero)
            {
                if (c2 < zero && a1 < zero)
                {
                    // Four simple real roots.
                    rmList->push_back(1);
                    rmList->push_back(1);
                    rmList->push_back(1);
                    rmList->push_back(1);
                }
                else // c2 >= 0 or a1 >= 0
                {
                    // Two complex-conjugate pairs.
                }
            }
            else if (delta < zero)
            {
                // Two simple real roots, one complex-conjugate pair.
                rmList->push_back(1);
                rmList->push_back(1);
            }
            else  // delta = 0
            {
                if (a1 > zero || (c2 > zero && (a1 != zero || c1 != zero)))
                {
                    // One double real root, one complex-conjugate pair.
                    rmList->push_back(2);
                }
                else
                {
                    if (a0 != zero)
                    {
                        // One double real root, two simple real roots.
                        rmList->push_back(2);
                        rmList->push_back(1);
                        rmList->push_back(1);
                    }
                    else
                    {
                        // One triple real root, one simple real root.
                        rmList->push_back(3);
                        rmList->push_back(1);
                    }
                }
            }

            return (int)rmList->size();
        }

        template<typename T>
        static int GetNumRootsBiquadratic(const T& c0, const T& c2, std::vector<int>* rmList)
        {
            static std::vector<int> _tmp;
            _tmp.clear();
            rmList = rmList == nullptr ? &_tmp : rmList;

            // Solve 0 = x^4 + c2*x^2 + c0 = (x^2 + c2/2)^2 + a1, where
            // a1 = c0 - c2^2/2.  We know that c0 != 0 at the time of the function
            // call, so x = 0 is not a root.  The condition c1 = 0 implies the quartic
            // Delta = 256*c0*a1^2.
            const T zero = 0, rat2 = 2, rat256 = 256;
            T c2Half = c2 / rat2;
            T a1 = c0 - c2Half * c2Half;
            T delta = rat256 * c0 * a1 * a1;
            if (delta > zero)
            {
                if (c2 < zero)
                {
                    if (a1 < zero)
                    {
                        // Four simple roots.
                        rmList->push_back(1);
                        rmList->push_back(1);
                        rmList->push_back(1);
                        rmList->push_back(1);
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
                rmList->push_back(1);
                rmList->push_back(1);
            }
            else  // delta = 0
            {
                if (c2 < zero)
                {
                    // Two double real roots.
                    rmList->push_back(2);
                    rmList->push_back(2);
                }
                else  // c2 > 0
                {
                    // Two double complex conjugate pairs.
                }
            }

            return (int)rmList->size();
        }


        template<typename T>
        static int FindRoots(const std::vector<T>& coeffs, std::vector<T>* roots, int maxIerations)
        {
            int degree = (int)coeffs.size();
            T* c = coeffs.data();

            if (degree >= 0 && c)
            {
                const T zero = (T)0;
                std::vector<T> depressedCoeffs = coeffs;
                while (degree >= 0 && c[degree] == zero)
                {
                    --degree;
                    depressedCoeffs.pop_back();
                }
                assert(depressedCoeffs.size() == degree);

                if (degree > 0)
                {
                    // Compute the Cauchy bound.
                    const T one = (T)1;
                    T invLeading = one / c[degree];
                    T maxValue = zero;
                    for (int i = 0; i < degree; ++i)
                    {
                        T value = std::abs(c[i] * invLeading);
                        if (value > maxValue)
                        {
                            maxValue = value;
                        }
                    }
                    T bound = one + maxValue;

                    return internal::FindRootsRecursive(depressedCoeffs, 
                                                        roots, 
                                                        maxIterations, 
                                                        -bound,
                                                        bound);
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


        template<typename T>
        static bool FindRootsInside(const std::vector<T>& coeffs, T* root, int maxIerations, T tmin, T tmax)
        {
            const T zero = (T)0;
            T pmin = internal::Evaluate(coeffs, tmin);
            if (pmin == zero)
            {
                *root = tmin;
                return true;
            }
            T pmax = internal::Evaluate(coeffs, tmax);
            if (pmax == zero)
            {
                *root = tmax;
                return true;
            }

            if (pmin*pmax > zero)
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
                *root = ((T)0.5) * (tmin + tmax);

                // This test is designed for 'float' or 'double' when tmin and tmax
                // are consecutive floating-point numbers.
                if (*root == tmin || *root == tmax)
                {
                    break;
                }

                T p = internal::Evaluate(coeffs, root);
                T product = p * pmin;
                if (product < zero)
                {
                    tmax = *root;
                    pmax = p;
                }
                else if (product > zero)
                {
                    tmin = *root;
                    pmin = p;
                }
                else
                {
                    break;
                }
            }

            return true;
        }

        template<typename T>
        static int FindRootsRecursive(int degree, const T* c, T tmin, T tmax, int maxIterations, T* roots)
        {
            // The base of the recursion.
            const T zero = (T)0;
            T root = zero;
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
            std::vector<T> derivCoeff(derivDegree + 1);
            std::vector<T> derivRoots(derivDegree);
            for (int i = 0; i <= derivDegree; ++i)
            {
                derivCoeff[i] = c[i + 1] * (T)(i + 1) / (T)degree;
            }
            int numDerivRoots = internal::FindRootsRecursive(degree - 1, 
                                                             &derivCoeff[0], 
                                                             tmin, 
                                                             tmax,
                                                             maxIterations, 
                                                             &derivRoots[0]);

            int numRoots = 0;
            if (numDerivRoots > 0)
            {
                // Find root on [tmin,derivRoots[0]].
                if (PolynomialRoots<T>::FindRootInside(degree, 
                                                    c, 
                                                    tmin, 
                                                    derivRoots[0], 
                                                    maxIterations, 
                                                    &root))
                {
                    roots[numRoots++] = root;
                }

                // Find root on [derivRoots[i],derivRoots[i+1]].
                for (int i = 0; i <= numDerivRoots - 2; ++i)
                {
                    if (PolynomialRoots<T>::FindRootInside(degree, 
                                                           c,
                                                           derivRoots[i], 
                                                           derivRoots[i + 1],
                                                           maxIterations, 
                                                           &root))
                    {
                        roots[numRoots++] = root;
                    }
                }

                // Find root on [derivRoots[numDerivRoots-1],tmax].
                if (PolynomialRoots<T>::FindRootInside(degree, 
                                                       c, 
                                                       derivRoots[numDerivRoots - 1], 
                                                       tmax,
                                                       maxIterations, 
                                                       &root))
                {
                    roots[numRoots++] = root;
                }
            }
            else
            {
                // The polynomial is monotone on [tmin,tmax], so has at most one root.
                if (PolynomialRoots<T>::FindRootInside(degree, 
                                                       c, 
                                                       tmin, 
                                                       tmax, 
                                                       maxIterations, 
                                                       &root))
                {
                    roots[numRoots++] = root;
                }
            }
            return numRoots;
        }

        template<typename T>
        static T Evaluate(int degree, const T* c, T t)
        {
            int i = degree;
            T result = c[i];
            while (--i >= 0)
            {
                result = t * result + c[i];
            }
            return result;
        }
    } // internal
} // GA
