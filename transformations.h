#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

Eigen::Vector4d convertZYX2Q(const Eigen::Vector3d& zyx);

Eigen::Vector3d convertQ2ZYX(const Eigen::Vector4d& q);

Eigen::Matrix3d getRotation(const Eigen::Vector3d& zyx);

Eigen::Vector3d getRotation(const Eigen::Matrix3d& R);

Eigen::Matrix3d getRotation(const Eigen::Vector4d& q);

Eigen::Vector4d getRotationQuat(const Eigen::Matrix3d& R);

Eigen::Matrix4d transform(const Eigen::Matrix4d& T1, const Eigen::Matrix4d& T2);

Eigen::Vector3d transform(const Eigen::Matrix4d& T, const Eigen::Vector3d& p);

#endif