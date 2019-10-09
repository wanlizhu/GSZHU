#pragma once

#include "AxisAngle.h"
#include "EulerAngle.h"
#include <variant>
#include <type_traits>

#define ORIGINAL_ROT (mRotationType==ERotationType::Matrix      ? mMatrix      :\
                     (mRotationType==ERotationType::AxisAngle   ? mAxisAngle   :\
                     (mRotationType==ERotationType::EulerAngles ? mEulerAngles :\
                     (mRotationType==ERotationType::Quaternion  ? mQuaternion  : nullptr))))

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




    template<typename T, int N>
    Rotation<T, N>::Rotation(const Rotation<T, N>::MAT& mat)
        : mRotationType(ERotationType::Matrix)
        , mMatrix(mat)
    {
        mIsComputed[ENUM2INT(ERotationType::Matrix)] = true;
    }

    template<typename T, int N>
    Rotation<T, N>::Rotation(const Rotation<T, N>::AA& aa)
        : mRotationType(ERotationType::AxisAngle)
        , mAxisAngle(aa)
    {
        mIsComputed[ENUM2INT(ERotationType::AxisAngle)] = true;
    }

    template<typename T, int N>
    Rotation<T, N>::Rotation(const Rotation<T, N>::EA& ea)
        : mRotationType(ERotationType::EulerAngles)
        , mEulerAngles(ea)
    {
        mIsComputed[ENUM2INT(ERotationType::EulerAngles)] = true;
    }

    template<typename T, int N>
    Rotation<T, N>::Rotation(const Rotation<T, N>::QUAT& quat)
        : mRotationType(ERotationType::Quaternion)
        , mQuaternion(quat)
    {
        mIsComputed[ENUM2INT(ERotationType::Quaternion)] = true;
    }

    template<typename T, int N>
    Rotation<T, N>::operator const MAT& () const
    {
        if (!mIsComputed[ENUM2INT(ERotationType::Matrix)])
        {
            Rotation::ToMatrix(ORIGINAL_ROT, &mMatrix);
            mIsComputed[ENUM2INT(ERotationType::Matrix)] = true;
        }
        return mMatrix;
    }

    template<typename T, int N>
    Rotation<T, N>::operator const AA& () const
    {
        if (!mIsComputed[ENUM2INT(ERotationType::AxisAngle)])
        {
            Rotation::ToAxisAngle(ORIGINAL_ROT, &mAxisAngle);
            mIsComputed[ENUM2INT(ERotationType::AxisAngle)] = true;
        }
        return mAxisAngle;
    }

    template<typename T, int N>
    Rotation<T, N>::operator const EA& () const
    {
        if (!mIsComputed[ENUM2INT(ERotationType::EulerAngles)])
        {
            Rotation::ToEulerAngles(ORIGINAL_ROT, &mEulerAngles);
            mIsComputed[ENUM2INT(ERotationType::EulerAngles)] = true;
        }
        return mEulerAngles;
    }

    template<typename T, int N>
    Rotation<T, N>::operator const QUAT& () const
    {
        if (!mIsComputed[ENUM2INT(ERotationType::Quaternion)])
        {
            Rotation::ToQuaternion(ORIGINAL_ROT, &mQuaternion);
            mIsComputed[ENUM2INT(ERotationType::Quaternion)] = true;
        }
        return mQuaternion;
    }

    // Convert a quaterion q = x*i + y*j + z*k + w to a rotation matrix.
    // [GTE_USE_MAT_VEC]
    //     +-           -+   +-                                     -+
    // R = | r00 r01 r02 | = | 1-2y^2-2z^2  2(xy-zw)     2(xz+yw)    |
    //     | r10 r11 r12 |   | 2(xy+zw)     1-2x^2-2z^2  2(yz-xw)    |
    //     | r20 r21 r22 |   | 2(xz-yw)     2(yz+xw)     1-2x^2-2y^2 |
    //     +-           -+   +-                                     -+
    // [GTE_USE_VEC_MAT]
    //     +-           -+   +-                                     -+
    // R = | r00 r01 r02 | = | 1-2y^2-2z^2  2(xy+zw)     2(xz-yw)    |
    //     | r10 r11 r12 |   | 2(xy-zw)     1-2x^2-2z^2  2(yz+xw)    |
    //     | r20 r21 r22 |   | 2(xz+yw)     2(yz-xw)     1-2x^2-2y^2 |
    //     +-           -+   +-                                     -+
    template<typename T, int N>
    void Rotation<T, N>::ToMatrix(const QUAT& quat, MAT* m)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        assert(mat != nullptr);
        MAT& mat = *m;

        mat.setIdentity();

        T twoX = ((T)2) * quat[0];
        T twoY = ((T)2) * quat[1];
        T twoZ = ((T)2) * quat[2];
        T twoXX = twoX * quat[0];
        T twoXY = twoX * quat[1];
        T twoXZ = twoX * quat[2];
        T twoXW = twoX * quat[3];
        T twoYY = twoY * quat[1];
        T twoYZ = twoY * quat[2];
        T twoYW = twoY * quat[3];
        T twoZZ = twoZ * quat[2];
        T twoZW = twoZ * quat[3];

        mat(0, 0) = (T)1 - twoYY - twoZZ;
        mat(0, 1) = twoXY - twoZW;
        mat(0, 2) = twoXZ + twoYW;
        mat(1, 0) = twoXY + twoZW;
        mat(1, 1) = (T)1 - twoXX - twoZZ;
        mat(1, 2) = twoYZ - twoXW;
        mat(2, 0) = twoXZ - twoYW;
        mat(2, 1) = twoYZ + twoXW;
        mat(2, 2) = (T)1 - twoXX - twoYY;
    }

    template<typename T, int N>
    void Rotation<T, N>::ToMatrix(const EA& ea, MAT* mat);

    template<typename T, int N>
    void Rotation<T, N>::ToMatrix(const AA& aa, MAT* mat);

    // Convert a rotation matrix to a quaternion.
    //
    // x^2 = (+r00 - r11 - r22 + 1)/4
    // y^2 = (-r00 + r11 - r22 + 1)/4
    // z^2 = (-r00 - r11 + r22 + 1)/4
    // w^2 = (+r00 + r11 + r22 + 1)/4
    // x^2 + y^2 = (1 - r22)/2
    // z^2 + w^2 = (1 + r22)/2
    // y^2 - x^2 = (r11 - r00)/2
    // w^2 - z^2 = (r11 + r00)/2
    // x*y = (r01 + r10)/4
    // x*z = (r02 + r20)/4
    // y*z = (r12 + r21)/4
    // [GTE_USE_MAT_VEC]
    //   x*w = (r21 - r12)/4
    //   y*w = (r02 - r20)/4
    //   z*w = (r10 - r01)/4
    // [GTE_USE_VEC_MAT]
    //   x*w = (r12 - r21)/4
    //   y*w = (r20 - r02)/4
    //   z*w = (r01 - r10)/4
    //
    // If Q is the 4x1 column vector (x,y,z,w), the previous equations give us
    //         +-                  -+
    //         | x*x  x*y  x*z  x*w |
    // Q*Q^T = | y*x  y*y  y*z  y*w |
    //         | z*x  z*y  z*z  z*w |
    //         | w*x  w*y  w*z  w*w |
    //         +-                  -+
    // The code extracts the row of maximum length, normalizing it to obtain
    // the result q.
    template<typename T, int N>
    void Rotation<T, N>::ToQuaternion(const MAT& mat, QUAT* q)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        assert(quat != nullptr);
        QUAT& quat = *q;

        T r22 = mat(2, 2);
        if (r22 <= (T)0)  // x^2 + y^2 >= z^2 + w^2
        {
            T dif10 = mat(1, 1) - mat(0, 0);
            T omr22 = (T)1 - r22;
            if (dif10 <= (T)0)  // x^2 >= y^2
            {
                T fourXSqr = omr22 - dif10;
                T inv4x = ((T)0.5) / std::sqrt(fourXSqr);
                quat[0] = fourXSqr * inv4x;
                quat[1] = (mat(0, 1) + mat(1, 0)) * inv4x;
                quat[2] = (mat(0, 2) + mat(2, 0)) * inv4x;
                quat[3] = (mat(2, 1) - mat(1, 2)) * inv4x;
            }
            else  // y^2 >= x^2
            {
                T fourYSqr = omr22 + dif10;
                T inv4y = ((T)0.5) / std::sqrt(fourYSqr);
                quat[0] = (mat(0, 1) + mat(1, 0)) * inv4y;
                quat[1] = fourYSqr * inv4y;
                quat[2] = (mat(1, 2) + mat(2, 1)) * inv4y;
                quat[3] = (mat(0, 2) - mat(2, 0)) * inv4y;
            }
        }
        else  // z^2 + w^2 >= x^2 + y^2
        {
            T sum10 = mat(1, 1) + mat(0, 0);
            T opr22 = (T)1 + r22;
            if (sum10 <= (T)0)  // z^2 >= w^2
            {
                T fourZSqr = opr22 - sum10;
                T inv4z = ((T)0.5) / std::sqrt(fourZSqr);
                quat[0] = (mat(0, 2) + mat(2, 0)) * inv4z;
                quat[1] = (mat(1, 2) + mat(2, 1)) * inv4z;
                quat[2] = fourZSqr * inv4z;
                quat[3] = (mat(1, 0) - mat(0, 1)) * inv4z;
            }
            else  // w^2 >= z^2
            {
                T fourWSqr = opr22 + sum10;
                T inv4w = ((T)0.5) / std::sqrt(fourWSqr);
                quat[0] = (mat(2, 1) - mat(1, 2)) * inv4w;
                quat[1] = (mat(0, 2) - mat(2, 0)) * inv4w;
                quat[2] = (mat(1, 0) - mat(0, 1)) * inv4w;
                quat[3] = fourWSqr * inv4w;
            }
        }
    }

    template<typename T, int N>
    void Rotation<T, N>::ToQuaternion(const EA& ea, QUAT* quat);

    template<typename T, int N>
    void Rotation<T, N>::ToQuaternion(const AA& aa, QUAT* quat);

    template<typename T, int N>
    void Rotation<T, N>::ToAxisAngle(const MAT& mat, AA* aa);

    template<typename T, int N>
    void Rotation<T, N>::ToAxisAngle(const EA& ea, AA* aa);

    template<typename T, int N>
    void Rotation<T, N>::ToAxisAngle(const QUAT& quat, AA* aa);

    template<typename T, int N>
    void Rotation<T, N>::ToEulerAngles(const MAT& mat, EA* ea);

    template<typename T, int N>
    void Rotation<T, N>::ToEulerAngles(const AA& aa, EA* ea);

    template<typename T, int N>
    void Rotation<T, N>::ToEulerAngles(const QUAT& quat, EA* ea);
}