#include <Eigen/Eigen>

namespace GSZHU {
    typedef Eigen::Vector2i Vec2i;
    typedef Eigen::Vector3i Vec3i;
    typedef Eigen::Vector4i Vec4i;
    typedef Eigen::Vector2f Vec2f;
    typedef Eigen::Vector3f Vec3f;
    typedef Eigen::Vector4f Vec4f;
    typedef Eigen::Vector2d Vec2d;
    typedef Eigen::Vector3d Vec3d;
    typedef Eigen::Vector4d Vec4d;

    typedef Eigen::Matrix2i Mat2i;
    typedef Eigen::Matrix3i Mat3i;
    typedef Eigen::Matrix4i Mat4i;
    typedef Eigen::Matrix2f Mat2f;
    typedef Eigen::Matrix3f Mat3f;
    typedef Eigen::Matrix4f Mat4f;
    typedef Eigen::Matrix2d Mat2d;
    typedef Eigen::Matrix3d Mat3d;
    typedef Eigen::Matrix4d Mat4d;

    template<typename T, int ROWS, int COLS>
    using Matrix = Eigen::Matrix<T, ROWS, COLS>;

    typedef Eigen::MatrixXd DynamicMatrix;
}