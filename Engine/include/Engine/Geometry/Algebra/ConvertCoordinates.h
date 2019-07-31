#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    /* Two types of changing between coordinates:
     * U and V are two coordinate systems, X is in U, Y is in V.
     *    - Linear Change of Basis
     *      U and V, both bases provide bookkeeping information for 
     *      representing points in the common world. 
     *      To convert from the first coordinate system to the second is a matter of 
     *      converting the rst to the common world's coordinate system and then 
     *      converting the common world's coordinate system to the second coordinate system.
     *      So, Y = V^{-1}*U*X, X = U^{-1}*V*Y        (1)
     *      Define, C = U^{-1}*V, equation (1) becomes Y = C^{-1}*X, X = C*Y
     *
     *      The process is referred to as a 'linear change of basis'. 
     *      The matrix C represents that change. Although linear algebra books refer to this
     *      solely as 'change of basis', I added the modifier 'linear' to distinguish this from
     *      coordinate system conversions when the two systems have origins not equal to 
     *      the standard origin of the common world. In the latter case, I call the related process
     *      an 'affine change of basis'.
     *
     *         - Conversion of Linear Transformations
     *           M' = C^{-1}*M*C
     *           This is referred to as 'similarity transformations'. 
     *           Generally, given a matrix transformation M relative to the U-basis
     *           and the change of basis matrix C to convert to the V-basis, the matrix transformation
     *           in the latter basis is M' = C^{-1}*M*C
     *           PS:       X' = M*X
     *              C*Y' = X' = M*X = M*C*Y
     *                     Y' = C^{-1}*M*C*Y
     *
     *    - Affine Change of Basis
     *      Let E the origin of U, F the origin of V
     *      In matrix-vector form, the representations are P=E+U*X and P=F+V*Y.
     *      For matrix-vector notation, we may use 4x4 matrices written in block form and store the
     *      point representations in homogenous form,
     *              |U  E| |X|   |P|   |V  F| |Y|
     *              |0  1|*|1| = |1| = |0  1|*|1|
     *      We may solve for either coordinate vector by inversion,
     *              |Y|          |X|  |X|     |Y|
     *              |1| = A^{-1}*|1|, |1| = A*|1|        (2)
     *      where
     *                   |U  E|-1 |V  F|   |U^{-1}*V  U^{-1}*(F-E)|
     *          A      = |0  1|  *|0  1| = |0         1           |
     *      and
     *                   |V  F|-1 |U  E|   |V^{-1}*U  V^{-1}*(E-F)|
     *          A^{-1} = |0  1|  *|0  1| = |0         1           |
     *
     *         - Conversion of Linear Transformations
     *           An affine transformation for the first coordinate system is X' = M*X + T.
     *           In homogeneous form,
     *                   |X'|   |M  T| |X|     |X|
     *                   |1 | = |0  1|*|1| = H*|1|
     *           where the last equality defines the 4x4 matrix H. 
     */
     template<typename T, int N>
     class ConvertCoordinates
     {
     public:
         ConvertCoordinates();
         
         /* Compute a change of basis between two coordinate systems.  
          * The return value is 'true' iff U and V are invertible.  
          * The matrix-vector multiplication conventions,
          *     for column major matrix, put the vector on the right,
          *     for    row major matrix, put the vector on the left,
          * which affects the conversion of matrix transformations.
          */
         bool operator()(const Matrix<T, N, N>& U, const Matrix<T, N, N>& V);

         /* Convert points between coordinate systems.
          * The names of the systems are U and V to make it clear
          * which inputs of operator() they are associated with.
          * The X vector stores coordinates for the U-system and
          * the Y vector stores coordinates for the V-system.
          */
         inline Vector<T, N> U2V(const Vector<T, N>& X /*in U*/) const { return mInvC * X; }
         inline Vector<T, N> V2U(const Vector<T, N>& Y /*in V*/) const { return mC * Y; }

         /* Convert transformations between coordinate systems.  
          * The outputs are computed according to the tables shown below.
          * The superscript T denotes the transpose operator.
          * if U is column major : transformation is X' = A*X
          * if U is    row major : transformation is (X')^T = X^T*A
          * if V is column major : transformation is Y' = B*Y
          * if V is    row major : transformation is (Y')^T = Y^T*B
          *
          * U is column major | V is column major | Output
          * ------------------+-------------------+---------------------
          * true              | true              |  C^{-1} * A   * C
          * true              | false             | (C^{-1} * A   * C)^T 
          * false             | true              |  C^{-1} * A^T * C
          * false             | false             | (C^{-1} * A^T * C)^T
          */
         Matrix<T, N, N> U2V(const Matrix<T, N, N>& A) const;
         /* U is column major | V is column major | Output
          * ------------------+-------------------+---------------------
          * true              | true              |  C * B   * C^{-1}
          * true              | false             |  C * B^T * C^{-1}
          * false             | true              | (C * B   * C^{-1})^T
          * false             | false             | (C * B^T * C^{-1})^T
          */
         Matrix<T, N, N> V2U(const Matrix<T, N, N>& B) const;

         inline const Matrix<T, N, N>& GetC()    const { return mC; }
         inline const Matrix<T, N, N>& GetInvC() const { return mInvC; }
         inline                   bool IsRH_U()  const { return mIsRH_U; }
         inline                   bool IsRH_V()  const { return mIsRH_V; }
         inline          bool IsColumnMajor_U()  const { return mIsColumnMajor_U; }
         inline          bool IsColumnMajor_V()  const { return mIsColumnMajor_V; }

     private:
         Matrix<T, N, N> mC;    // C = U^{-1}*V
         Matrix<T, N, N> mInvC; // C^{-1} = V^{-1}*U
         
         bool mIsColumnMajor_U = true; 
         bool mIsColumnMajor_V = true;
         bool mIsRH_U = true; // Is U Right-handed?
         bool mIsRH_V = true; // Is V Right-handed?
     };











/*
                     TEMPLATE CLASS IMPLEMENTATION                  
  -------------------------------+----------------------------------
                           CLASS : ConvertCoordinates
                       ARGUMENTS : <typename T, int N>
                     GENERAL IMP : YES
          PARTIAL SPECIALIZATION : NO
             FULL SPECIALIZATION : NO
  -------------------------------+----------------------------------
*/

     template<typename T, int N>
     ConvertCoordinates<T, N>::ConvertCoordinates()
         :    mC(Matrix<T, N, N>::Identity())
         , mInvC(Matrix<T, N, N>::Identity())
         , mIsColumnMajor_U(true)
         , mIsColumnMajor_V(true)
         , mIsRH_U(true)
         , mIsRH_V(true)
     {}


     template<typename T, int N>
     bool ConvertCoordinates<T, N>::operator()(const Matrix<T, N, N>& U, const Matrix<T, N, N>& V)
     {
         if (0 < N && N <= 4)
         {
             Matrix<T, N, N> invU, invV;
             T detU, detV;
             bool invertibleU = false;
             bool invertibleV = false;

             U.computeInverseAndDetWithCheck(invU, detU, invertibleU);
             if (!invertibleU)
                 return false;
             V.computeInverseAndDetWithCheck(invV, detV, invertibleV);
             if (!invertibleV)
                 return false;

             mC = invU * V;
             mInvC = invV * U;
             mIsColumnMajor_U = Matrix<T, N, N>::Options == Eigen::ColMajor;
             mIsColumnMajor_V = Matrix<T, N, N>::Options == Eigen::ColMajor;
             mIsRH_U = detU > (T)0;
             mIsRH_V = detV > (T)0;
         }
         else
         {
             if (!Eigen::FullPivLU<Matrix<T, N, N>>(U).isInvertible() ||
                 !Eigen::FullPivLU<Matrix<T, N, N>>(V).isInvertible())
                 return false;

             mC = U.inverse() * V;
             mInvC = V.inverse() * U;
             mIsColumnMajor_U = Matrix<T, N, N>::Options == Eigen::ColMajor;
             mIsColumnMajor_V = Matrix<T, N, N>::Options == Eigen::ColMajor;
             mIsRH_U = U.determinant() > (T)0;
             mIsRH_V = V.determinant() > (T)0;
         }

         return true;
     }


     template<typename T, int N>
     Matrix<T, N, N> ConvertCoordinates<T, N>::U2V(const Matrix<T, N, N>& A) const
     {
         Matrix<T, N, N> product;
         if (mIsColumnMajor_U){
             product = mInvC * A * mC;
             if (mIsColumnMajor_V){
                 return product;
             }
             else {
                 return product.transpose();
             }
         }
         else {
             product = mInvC * A.transpose() * mC;
             if (mIsColumnMajor_V) {
                 return product;
             }
             else {
                 return product.transpose();
             }
         }
     }


     template<typename T, int N>
     Matrix<T, N, N> ConvertCoordinates<T, N>::V2U(const Matrix<T, N, N>& B) const
     {
         Matrix<T, N, N> product;
         if (mIsColumnMajor_V) {
             product = mC * B * mInvC;
             if (mIsColumnMajor_U) {
                 return product;
             }
             else {
                 return product.transpose();
             }
         }
         else {
             product = mC * B.transpose() * mInvC;
             if (mIsColumnMajor_U) {
                 return product;
             }
             else {
                 product.transpose();
             }
         }
     }
}