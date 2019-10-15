#include "GE2/CGA/internal/Polynomial.h"

namespace CGA
{
    Polynomial::Polynomial(unsigned int degree)
        : mCoefficient(degree + 1, 0.0)
    {}

    Polynomial::Polynomial(std::initializer_list<double> values)
    {
        mCoefficient.resize(values.size());
        std::copy(values.begin(), values.end(), mCoefficient.begin());
        EliminateLeadingZeros();
    }

    void Polynomial::SetDegree(unsigned int degree)
    {
        mCoefficient.resize(degree + 1);
    }

    void Polynomial::SetCoefficients(double value)
    {
        std::fill(mCoefficient.begin(), mCoefficient.end(), value);
    }

    double Polynomial::operator()(double t) const
    {
        int i = static_cast<int>(mCoefficient.size());
        double result = mCoefficient[--i];
        for (--i; i >= 0; --i)
        {
            result *= t;
            result += mCoefficient[i];
        }
        return result;
    }

    Polynomial Polynomial::GetDerivative() const
    {
        unsigned int const degree = GetDegree();
        if (degree > 0)
        {
            Polynomial result(degree - 1);
            for (unsigned int i0 = 0, i1 = 1; i0 < degree; ++i0, ++i1)
            {
                result.mCoefficient[i0] = mCoefficient[i1] * (double)i1;
            }
            return result;
        }
        else
        {
            Polynomial result(0);
            result[0] = 0.0;
            return result;
        }
    }

    Polynomial Polynomial::GetInversion() const
    {
        unsigned int const degree = GetDegree();
        Polynomial result(degree);
        for (unsigned int i = 0; i <= degree; ++i)
        {
            result.mCoefficient[i] = mCoefficient[degree - i];
        }
        return result;
    }

    Polynomial Polynomial::GetTranslation(double t0) const
    {
        Polynomial factor{ -t0, 1.0 };  // f(t) = t - t0
        unsigned int const degree = GetDegree();
        Polynomial result{ mCoefficient[degree] };
        for (unsigned int i = 1, j = degree - 1; i <= degree; ++i, --j)
        {
            result = mCoefficient[j] + factor * result;
        }
        return result;
    }

    void Polynomial::EliminateLeadingZeros()
    {
        size_t size = mCoefficient.size();
        if (size > 1)
        {
            double const zero = 0.0;
            int leading;
            for (leading = static_cast<int>(size) - 1; leading > 0; --leading)
            {
                if (mCoefficient[leading] != zero)
                {
                    break;
                }
            }

            mCoefficient.resize(++leading);
        }
    }

    void Polynomial::Divide(const Polynomial& divisor,
                            Polynomial& quotient,
                            Polynomial& remainder) const
    {
        double const zero = 0.0;
        int divisorDegree = static_cast<int>(divisor.GetDegree());
        int quotientDegree = static_cast<int>(GetDegree()) - divisorDegree;
        if (quotientDegree >= 0)
        {
            quotient.SetDegree(quotientDegree);

            // Temporary storage for the remainder.
            Polynomial tmp = *this;

            // Do the division using the Euclidean algorithm.
            double inv = (1.0) / divisor[divisorDegree];
            for (int i = quotientDegree; i >= 0; --i)
            {
                int j = divisorDegree + i;
                quotient[i] = inv * tmp[j];
                for (j--; j >= i; j--)
                {
                    tmp[j] -= quotient[i] * divisor[j - i];
                }
            }

            // Calculate the correct degree for the remainder.
            if (divisorDegree >= 1)
            {
                int remainderDegree = divisorDegree - 1;
                while (remainderDegree > 0 && tmp[remainderDegree] == zero)
                {
                    --remainderDegree;
                }

                remainder.SetDegree(remainderDegree);
                for (int i = 0; i <= remainderDegree; ++i)
                {
                    remainder[i] = tmp[i];
                }
            }
            else
            {
                remainder.SetDegree(0);
                remainder[0] = zero;
            }
        }
        else
        {
            quotient.SetDegree(0);
            quotient[0] = zero;
            remainder = *this;
        }
    }

    void Polynomial::MakeMonic()
    {
        EliminateLeadingZeros();
        double const one(1);
        if (mCoefficient.back() != one)
        {
            unsigned int degree = GetDegree();
            double invLeading = one / mCoefficient.back();
            mCoefficient.back() = one;
            for (unsigned int i = 0; i < degree; ++i)
            {
                mCoefficient[i] *= invLeading;
            }
        }
    }

    Polynomial GreatestCommonDivisor(const Polynomial& p0, const Polynomial& p1)
    {
        // The numerator should be the polynomial of larger degree.
        Polynomial a, b;
        if (p0.GetDegree() >= p1.GetDegree())
        {
            a = p0;
            b = p1;
        }
        else
        {
            a = p1;
            b = p0;
        }

        Polynomial const zero{ 0.0 };
        if (a == zero || b == zero)
        {
            return (a != zero ? a : zero);
        }

        // Make the polynomials monic to keep the coefficients reasonable size
        // when computing with floating-point Real.
        a.MakeMonic();
        b.MakeMonic();

        Polynomial q, r;
        for (;;)
        {
            a.Divide(b, q, r);
            if (r != zero)
            {
                // a = q * b + r, so gcd(a,b) = gcd(b, r)
                a = b;
                b = r;
                b.MakeMonic();
            }
            else
            {
                b.MakeMonic();
                break;
            }
        }

        return b;
    }

    void SquareFreeFactorization(const Polynomial& f, std::vector<Polynomial>& factors)
    {
        // In the call to Divide(...), we know that the divisor exactly
        // divides the numerator, so r = 0 after all such calls.
        Polynomial fder = f.GetDerivative();
        Polynomial a, b, c, d, q, r;

        a = GreatestCommonDivisor(f, fder);
        f.Divide(a, b, r);  // b = f / a
        fder.Divide(a, c, r);  // c = fder / a
        d = c - b.GetDerivative();

        do
        {
            a = GreatestCommonDivisor(b, d);
            factors.emplace_back(a);
            b.Divide(a, q, r);  // q = b / a
            b = std::move(q);
            d.Divide(a, c, r);  // c = d / a
            d = c - b.GetDerivative();
        } while (b.GetDegree() > 0);
    }




    Polynomial operator-(const Polynomial& p)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        for (unsigned int i = 0; i <= degree; ++i)
        {
            result[i] = -p[i];
        }
        return result;
    }

    Polynomial operator+(const Polynomial& p0, const Polynomial& p1)
    {
        unsigned int const p0Degree = p0.GetDegree();
        unsigned int const p1Degree = p1.GetDegree();
        unsigned int i;
        if (p0Degree >= p1Degree)
        {
            Polynomial result(p0Degree);
            for (i = 0; i <= p1Degree; ++i)
            {
                result[i] = p0[i] + p1[i];
            }
            for (; i <= p0Degree; ++i)
            {
                result[i] = p0[i];
            }
            result.EliminateLeadingZeros();
            return result;
        }
        else
        {
            Polynomial result(p1Degree);
            for (i = 0; i <= p0Degree; ++i)
            {
                result[i] = p0[i] + p1[i];
            }
            for (; i <= p1Degree; ++i)
            {
                result[i] = p1[i];
            }
            result.EliminateLeadingZeros();
            return result;
        }
    }

    Polynomial operator-(const Polynomial& p0, const Polynomial& p1)
    {
        unsigned int const p0Degree = p0.GetDegree();
        unsigned int const p1Degree = p1.GetDegree();
        unsigned int i;
        if (p0Degree >= p1Degree)
        {
            Polynomial result(p0Degree);
            for (i = 0; i <= p1Degree; ++i)
            {
                result[i] = p0[i] - p1[i];
            }
            for (; i <= p0Degree; ++i)
            {
                result[i] = p0[i];
            }
            result.EliminateLeadingZeros();
            return result;
        }
        else
        {
            Polynomial result(p1Degree);
            for (i = 0; i <= p0Degree; ++i)
            {
                result[i] = p0[i] - p1[i];
            }
            for (; i <= p1Degree; ++i)
            {
                result[i] = -p1[i];
            }
            result.EliminateLeadingZeros();
            return result;
        }
    }

    Polynomial operator*(const Polynomial& p0, const Polynomial& p1)
    {
        unsigned int const p0Degree = p0.GetDegree();
        unsigned int const p1Degree = p1.GetDegree();
        Polynomial result(p0Degree + p1Degree);
        result.SetCoefficients((double)0);
        for (unsigned int i0 = 0; i0 <= p0Degree; ++i0)
        {
            for (unsigned int i1 = 0; i1 <= p1Degree; ++i1)
            {
                result[i0 + i1] += p0[i0] * p1[i1];
            }
        }
        return result;
    }

    Polynomial operator+(const Polynomial& p, double scalar)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        result[0] = p[0] + scalar;
        for (unsigned int i = 1; i <= degree; ++i)
        {
            result[i] = p[i];
        }
        return result;
    }

    Polynomial operator+(double scalar, const Polynomial& p)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        result[0] = p[0] + scalar;
        for (unsigned int i = 1; i <= degree; ++i)
        {
            result[i] = p[i];
        }
        return result;
    }

    Polynomial operator-(const Polynomial& p, double scalar)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        result[0] = p[0] - scalar;
        for (unsigned int i = 1; i <= degree; ++i)
        {
            result[i] = p[i];
        }
        return result;
    }

    Polynomial operator-(double scalar, const Polynomial& p)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        result[0] = scalar - p[0];
        for (unsigned int i = 1; i <= degree; ++i)
        {
            result[i] = -p[i];
        }
        return result;
    }

    Polynomial operator*(const Polynomial& p, double scalar)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        for (unsigned int i = 0; i <= degree; ++i)
        {
            result[i] = scalar * p[i];
        }
        return result;
    }

    Polynomial operator*(double scalar, const Polynomial& p)
    {
        unsigned int const degree = p.GetDegree();
        Polynomial result(degree);
        for (unsigned int i = 0; i <= degree; ++i)
        {
            result[i] = scalar * p[i];
        }
        return result;
    }

    Polynomial operator/(const Polynomial& p, double scalar)
    {
        assert(scalar != (double)0);

        unsigned int const degree = p.GetDegree();
        double invScalar = (1.0) / scalar;
        Polynomial result(degree);
        for (unsigned int i = 0; i <= degree; ++i)
        {
            result[i] = invScalar * p[i];
        }
        return result;
    }

    Polynomial& operator+=(Polynomial& p0, const Polynomial& p1)
    {
        p0 = p0 + p1;
        return p0;
    }

    Polynomial& operator-=(Polynomial& p0, const Polynomial& p1)
    {
        p0 = p0 - p1;
        return p0;
    }

    Polynomial& operator*=(Polynomial& p0, const Polynomial& p1)
    {
        p0 = p0 * p1;
        return p0;
    }

    Polynomial& operator+=(Polynomial& p, double scalar)
    {
        p[0] += scalar;
        return p;
    }

    Polynomial& operator-=(Polynomial& p, double scalar)
    {
        p[0] -= scalar;
        return p;
    }

    Polynomial& operator*=(Polynomial& p, double scalar)
    {
        p[0] *= scalar;
        return p;
    }

    Polynomial& operator/=(Polynomial& p, double scalar)
    {
        p[0] /= scalar;
        return p;
    }
}