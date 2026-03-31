#include "transformations.h"
#include <cmath>

using namespace Eigen;

Vector4d convertZYX2Q(const Vector3d& zyx) {
    double phi   = zyx(0);   // yaw
    double theta = zyx(1);   // pitch
    double psi   = zyx(2);   // roll

    Matrix3d R = (AngleAxisd(phi, Vector3d::UnitZ())
               * AngleAxisd(theta, Vector3d::UnitY())
               * AngleAxisd(psi, Vector3d::UnitX())).toRotationMatrix();
    Quaterniond q(R);
    q.normalize();
    return Vector4d(q.x(), q.y(), q.z(), q.w());
}

Vector3d convertQ2ZYX(const Vector4d& qv) {
    Quaterniond q(qv(3), qv(0), qv(1), qv(2));
    Matrix3d R = q.toRotationMatrix();
    return getRotation(R);
}

Matrix3d getRotation(const Vector3d& zyx) {
    double phi   = zyx(0);
    double theta = zyx(1);
    double psi   = zyx(2);
    return (AngleAxisd(phi, Vector3d::UnitZ())
         * AngleAxisd(theta, Vector3d::UnitY())
         * AngleAxisd(psi, Vector3d::UnitX())).toRotationMatrix();
}

Vector3d getRotation(const Matrix3d& R) {
    double phi   = atan2(R(1,0), R(0,0));               // yaw
    double theta = atan2(-R(2,0), sqrt(R(2,1)*R(2,1) + R(2,2)*R(2,2))); // pitch
    double psi   = atan2(R(2,1), R(2,2));               // roll
    return Vector3d(phi, theta, psi);
}

Matrix3d getRotation(const Vector4d& qv) {
    Quaterniond q(qv(3), qv(0), qv(1), qv(2));
    return q.toRotationMatrix();
}

Vector4d getRotationQuat(const Matrix3d& R) {
    Quaterniond q(R);
    q.normalize();
    return Vector4d(q.x(), q.y(), q.z(), q.w());
}

Matrix4d transform(const Matrix4d& T1, const Matrix4d& T2) {
    return T1 * T2;
}

Vector3d transform(const Matrix4d& T, const Vector3d& p) {
    Vector4d ph(p(0), p(1), p(2), 1.0);
    Vector4d pt = T * ph;
    return Vector3d(pt(0), pt(1), pt(2));
}