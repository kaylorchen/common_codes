#ifndef __PCL_ROTATE_H__
#define __PCL_ROTATE_H__

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/transforms.h>
#include "memory"

typedef struct {
  double x;
  double y;
  double yaw;
} RobotState;

class PclRotate {
public:
  PclRotate(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, RobotState &state){
    cloud_ = cloud;
    state_ = state;
  }
  void UpdatePclRotate(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, RobotState &state);

  pcl::PointCloud<pcl::PointXYZ>::Ptr
  transformPointCloud(const RobotState &current_state);

private:
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_;
  RobotState state_;
};

#endif