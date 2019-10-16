#pragma once

#include <algorithm>
#include <initializer_list>
#include <vector>
#include "GE2/Utilities.h"

namespace GE2::CGA
{
    class GE2_IMPEXP Polynomial final
    {
    public:
        // Construction and destruction.  The first constructor creates a
        // polynomial of the specified degree but sets all coefficients to
        // zero (to ensure initialization).  You are responsible for setting
        // the coefficients, presumably with the degree-term set to a nonzero
        // number.  In the second constructor, the degree is the number of
        // initializers plus 1, but then adjusted so that coefficient[degree]
        // is not zero (unless all initializer values are zero).
        Polynomial(unsigned int degree = 0);

        // C++ 11 will call the default constructor for
        // Polynomial1<Real> p{}, so it is guaranteed that
        // values.size() > 0.
        Polynomial(std::initializer_list<double> values);

        // Support for partial construction, where the default constructor is
        // used when the degree is not yet known.  The coefficients are
        // uninitialized.
        void SetDegree(unsigned int degree);

        // Set all coefficients to the specified value.
        void SetCoefficients(double value);

        // Member access
        inline unsigned int GetDegree() const        { return static_cast<unsigned int>(mCoefficient.size() - 1); }
        inline double& operator[](int i)             { return mCoefficient[i]; }
        inline const double& operator[](int i) const { return mCoefficient[i]; }

        // Comparisons
        inline bool operator==(const Polynomial& p) const { return mCoefficient == p.mCoefficient; }
        inline bool operator!=(const Polynomial& p) const { return !operator==(p); }
        inline bool operator< (const Polynomial& p) const { return mCoefficient < p.mCoefficient; }
        inline bool operator<=(const Polynomial& p) const { return mCoefficient <= p.mCoefficient; }
        inline bool operator> (const Polynomial& p) const { return mCoefficient > p.mCoefficient; }
        inline bool operator>=(const Polynomial& p) const { return mCoefficient >= p.mCoefficient; }

        // Evaluate the polynomial.  If the polynomial is invalid, the function returns zero.
        double operator()(double t) const;

        // Compute the derivative of the polynomial.
        Polynomial GetDerivative() const;

        // Inversion (invpoly[i] = poly[degree-i] for 0 <= i <= degree).
        Polynomial GetInversion() const;

        // Tranlation.  If 'this' is p(t}, return p(t-t0).
        Polynomial GetTranslation(double t0) const;

        // Eliminate any leading zeros in the polynomial, except in the case
        // the degree is 0 and the coefficient is 0.  The elimination is
        // necessary when arithmetic operations cause a decrease in the degree
        // of the result.  For example, (1 + x + x^2) + (1 + 2*x - x^2) =
        // (2 + 3*x).  The inputs both have degree 2, so the result is created
        // with degree 2.  After the addition we find that the degree is in
        // fact 1 and resize the array of coefficients.  This function is
        // called internally by the arithmetic operators, but it is exposed in
        // the public interface in case you need it for your own purposes.
        void EliminateLeadingZeros();

        // If 'this' is P(t) and the divisor is D(t) with
        // degree(P) >= degree(D), then P(t) = Q(t)*D(t)+R(t) where Q(t) is
        // the quotient with degree(Q) = degree(P) - degree(D) and R(t) is the
        // remainder with degree(R) < degree(D).  If this routine is called
        // with degree(P) < degree(D), then Q = 0 and R = P are returned.
        void Divide(const Polynomial& divisor,
                    Polynomial& quotient,
                    Polynomial& remainder) const;

        // Scale the polynomial so the highest-degree term has coefficient 1.
        void MakeMonic();

    private:
        // The class is designed so that mCoefficient.size() >= 1.
        std::vector<double> mCoefficient;
    };




    // Compute the greatest common divisor of two polynomials.  The returned
    // polynomial has leading coefficient 1 (except when zero-valued
    // polynomials are passed to the function.
    GE2_IMPEXP Polynomial GreatestCommonDivisor(const Polynomial& p0, const Polynomial& p1);

    // Factor f = factor[0]*factor[1]^2*factor[2]^3*...*factor[n-1]^n
    // according to the square-free factorization algorithm
    // https://en.wikipedia.org/wiki/Square-free_polynomial
    GE2_IMPEXP void SquareFreeFactorization(const Polynomial& f, std::vector<Polynomial>& factors);




    GE2_IMPEXP Polynomial  operator- (const Polynomial& p);
    GE2_IMPEXP Polynomial  operator+ (const Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial  operator- (const Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial  operator* (const Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial  operator+ (const Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial  operator+ (double scalar,        const Polynomial& p);
    GE2_IMPEXP Polynomial  operator- (const Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial  operator- (double scalar,        const Polynomial& p);
    GE2_IMPEXP Polynomial  operator* (const Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial  operator* (double scalar,        const Polynomial& p);
    GE2_IMPEXP Polynomial  operator/ (const Polynomial& p,  double scalar);

    GE2_IMPEXP Polynomial& operator+=(Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial& operator-=(Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial& operator*=(Polynomial& p0, const Polynomial& p1);
    GE2_IMPEXP Polynomial& operator+=(Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial& operator-=(Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial& operator*=(Polynomial& p,  double scalar);
    GE2_IMPEXP Polynomial& operator/=(Polynomial& p,  double scalar);
}