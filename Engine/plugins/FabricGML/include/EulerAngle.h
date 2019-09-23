#pragma once

#include "Vector.h"

namespace GML
{
	// Factorization into Euler angles is not necessarily unique. Let the
	// integer indices for the axes be (N0,N1,N2), which must be in the set
	//   {(0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0),
	//    (0,1,0),(0,2,0),(1,0,1),(1,2,1),(2,0,2),(2,1,2)}
	// Let the corresponding angles be (angleN0,angleN1,angleN2). If the
	// result is unique, then the multiple solutions occur because
	// angleN2+angleN0 is constant.  If the result is not unique diff, then
	// the multiple solutions occur because angleN2-angleN0 is constant. In
	// either type of nonuniqueness, the function returns angleN0=0.

}