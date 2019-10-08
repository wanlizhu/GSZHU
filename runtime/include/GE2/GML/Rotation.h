#pragma once

#include "AxisAngle.h"
#include "EulerAngle.h"
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

	template<typename T, int N>
	class Rotation
	{
        static_assert(N == 3 || N == 4);
	public:
		using MAT  = Matrix<T, N, N>;
		using QUAT = Quaternion<T>;
		using AA   = AxisAngle<T, N>;
		using EA   = EulerAngles<T>;

		Rotation(const MAT& mat);
		Rotation(const AA& axisAngle);
		Rotation(const EA& euler);
		Rotation(const QUAT& quat);

		operator MAT& ();
		operator AA&  ();
		operator EA&  ();
		operator QUAT&();

        operator const MAT& () const;
        operator const AA&  () const;
        operator const EA&  () const;
        operator const QUAT&() const;

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
        bool          mIsComputed[4] = { false };
		mutable MAT   mMatrix;
		mutable AA    mAxisAngle;
		mutable EA    mEulerAngles;
		mutable QUAT  mQuaternion;
	};


}