#include "GE2/CGA/internal/RootsBisection.h"

namespace GE2::CGA
{
    unsigned int RootsBisection::Find(const std::function<double(double)>& F,
                                      double t0, double t1,
                                      unsigned int maxIterations, double& root)
    {
        // Set 'root' initially to avoid "potentially uninitialized
        // variable" warnings by a compiler.
        root = t0;

        if (t0 < t1)
        {
            // Test the endpoints to see whether F(t) is zero.
            double f0 = F(t0);
            if (f0 == 0.0)
            {
                root = t0;
                return 1;
            }

            double f1 = F(t1);
            if (f1 == 0.0)
            {
                root = t1;
                return 1;
            }

            if (f0 * f1 > 0.0)
            {
                // It is not known whether the interval bounds a root.
                return 0;
            }

            unsigned int i;
            for (i = 2; i <= maxIterations; ++i)
            {
                root = 0.5 * (t0 + t1);
                if (root == t0 || root == t1)
                {
                    // The numbers t0 and t1 are consecutive
                    // floating-point numbers.
                    break;
                }

                double fm = F(root);
                double product = fm * f0;
                if (product < 0.0)
                {
                    t1 = root;
                    f1 = fm;
                }
                else if (product > 0.0)
                {
                    t0 = root;
                    f0 = fm;
                }
                else
                {
                    break;
                }
            }
            return i;
        }
        else
        {
            // The interval endpoints are invalid.
            return 0;
        }
    }

    unsigned int RootsBisection::Find(const std::function<double(double)>& F,
                                      double t0, double t1,
                                      double f0, double f1,
                                      unsigned int maxIterations, double& root)
    {
        // Set 'root' initially to avoid "potentially uninitialized
            // variable" warnings by a compiler.
        root = t0;

        if (t0 < t1)
        {
            // Test the endpoints to see whether F(t) is zero.
            if (f0 == 0.0)
            {
                root = t0;
                return 1;
            }

            if (f1 == 0.0)
            {
                root = t1;
                return 1;
            }

            if (f0 * f1 > 0.0)
            {
                // It is not known whether the interval bounds a root.
                return 0;
            }

            unsigned int i;
            root = t0;
            for (i = 2; i <= maxIterations; ++i)
            {
                root = 0.5 * (t0 + t1);
                if (root == t0 || root == t1)
                {
                    // The numbers t0 and t1 are consecutive
                    // floating-point numbers.
                    break;
                }

                double fm = F(root);
                double product = fm * f0;
                if (product < 0.0)
                {
                    t1 = root;
                    f1 = fm;
                }
                else if (product > 0.0)
                {
                    t0 = root;
                    f0 = fm;
                }
                else
                {
                    break;
                }
            }
            return i;
        }
        else
        {
            // The interval endpoints are invalid.
            return 0;
        }
    }
}