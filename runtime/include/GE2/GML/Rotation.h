#pragma once

#include "Matrix.h"
#include "AxisAngle.h"
#include "EulerAngle.h"
#include "Quaternion.h"
#include <variant>
#include <type_traits>

namespace GML
{
	enum class ERotationType
	{
		Matrix = 0,
		AxisAngle = 1,
		EulerAngles = 2,
		Quaternion = 3,
	};

	template<typename T, int N,
		     typename = std::enable_if_t<N == 3 || N == 4>>
	class Rotation
	{
	public:
		using MAT  = Matrix<T, N, N>;
		using QUAT = Quaternion<T>;
		using AA   = AxisAngle<T, N>;
		using EA   = EulerAngles<T>;

		Rotation(const MAT& mat);
		Rotation(const AA& axisAngle);
		Rotation(const EA& euler);
		Rotation(const QUAT& quat);

		operator MAT()  const;
		operator AA()   const;
		operator EA()   const;
		operator QUAT() const;

	private:
		static void ToMatrix(const QUAT& quat, MAT* mat);
		static void ToMatrix(const EA& ea,     MAT* mat);
		static void ToMatrix(const AA& aa,     MAT* mat);

		static void ToQuaternion(const MAT& mat, QUAT* quat);
		static void ToQuaternion(const EA& ea,   QUAT* quat);
		static void ToQuaternion(const AA& aa,   QUAT* quat);

		static void ToAxisAngle(const MAT& mat,   AA* aa);
		static void ToAxisAngle(const EA& ea,     AA* aa);
		static void ToAxisAngle(const QUAT& quat, AA* aa);

		static void ToEulerAngles(const MAT& mat,   EA* ea);
		static void ToEulerAngles(const AA& aa,     EA* ea);
		static void ToEulerAngles(const QUAT& quat, EA* ea);

	private:
		ERotationType mRotationType = ERotationType::Matrix;
		mutable Matrix<T, N, N> mMatrix;
		mutable AxisAngle<T, N> mAxisAngle;
		mutable EulerAngles<T>  mEulerAngles;
		mutable Quaternion<T>   mQuaternion;
	};


}