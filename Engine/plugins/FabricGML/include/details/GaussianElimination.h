#pragma once

#include <cmath>
#include <string>
#include <vector>

// The input matrix M must be NxN.  The storage convention for element lookup
// is determined by GML_USE_ROW_MAJOR, whichever is active.  
// If you want the inverse of M, pass a nonnull pointer inverseM;
// this matrix must also be NxN and use the same storage convention as M.  If
// you do not want the inverse of M, pass a nullptr for inverseM.  If you want
// to solve M*X = B for X, where X and B are Nx1, pass nonnull pointers for B
// and X.  If you want to solve M*Y = C for Y, where X and C are NxK, pass
// nonnull pointers for C and Y and pass K to numCols.  In all cases, pass
// N to numRows.

namespace GML
{
	template<typename T>
	class GaussianElimination
	{
	public:
		bool operator()(int numRows,
						const T* M, T* inverseM, T& detM,
						const T* B, T* X,
						const T* C, int numCols, T* Y) const;

	private:
		// Copy source to target or to set target to zero if source is nullptr.
		void set(int numElements, const T* source, T* target) const;
	};
}