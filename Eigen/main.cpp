#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>

int main() {
  Eigen::Matrix4d matrix_a = Eigen::Matrix4d::Identity();
  std::cout << "matrix_a:\n" << matrix_a <<  std::endl;
  Eigen::Quaterniond rotation =  Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitX())* Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitX());
  Eigen::Matrix3d rotation_matrix = rotation.toRotationMatrix().eval();
  std::cout << "rotation_matrix:\n" << rotation_matrix << std::endl;
  matrix_a.block<3, 3>(0,0) = rotation_matrix;
  std::cout << "matrix_a:\n" << matrix_a <<  std::endl;
  return 0;
}
