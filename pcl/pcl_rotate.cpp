#include "pcl_rotate.h"
pcl::PointCloud<pcl::PointXYZ>::Ptr
PclRotate::transformPointCloud(const RobotState &current_state) {
  double delta_x = current_state.x - this->state_.x;
  double delta_y = current_state.y - this->state_.y;
  double delta_yaw = current_state.yaw - this->state_.yaw;
  // 定义旋转角度和旋转轴
  Eigen::Affine3f transform = Eigen::Affine3f::Identity();
  // 平移向量
  transform.translation() << -delta_x, -delta_y, 0;
  // 旋转矩阵
  transform.rotate(Eigen::AngleAxisf(-delta_yaw, Eigen::Vector3f::UnitZ()));
  // 对点云进行旋转和平移
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_rotated(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::transformPointCloud(*(this->cloud_), *cloud_rotated, transform);
  return cloud_rotated;
}

void PclRotate::UpdatePclRotate(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, RobotState &state) {
  this->cloud_ = cloud;
  this->state_ = state;
}
