//
// Created by kaylor on 9/25/23.
//
#include "pcl_rotate.h"
#include <pcl/visualization/cloud_viewer.h>
#include "thread"

int main() {
  // 创建一个示例点云
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  cloud->width = 10;
  cloud->height = 1;
  cloud->points.resize(cloud->width * cloud->height);

  // 设置点云中的点的坐标
  for (size_t i = 0; i < cloud->points.size(); ++i) {
    cloud->points[i].x = static_cast<float>(i);
    cloud->points[i].y = static_cast<float>(i);
    cloud->points[i].z = static_cast<float>(i);
//    cloud->points[i].z = static_cast<float>(i);
  }

  RobotState robot_state ={0 , 0 , 0};
  PclRotate pcl_rotate(cloud, robot_state);
  auto cloud_rotated = pcl_rotate.transformPointCloud({20 , 0 , M_PI/4});

  pcl::visualization::PCLVisualizer::Ptr visualizer(
      new pcl::visualization::PCLVisualizer("PointCloud Visualizer"));
  visualizer->setBackgroundColor(0, 0, 0);
  visualizer->addPointCloud<pcl::PointXYZ>(cloud, "cloud1");
  visualizer->addPointCloud<pcl::PointXYZ>(cloud_rotated, "cloud2");
  visualizer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 0.0, 0.0,
                                               "cloud1"); // Set the color of cloud1 to red
  visualizer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0.0, 0.0, 1.0,
                                               "cloud2"); // Set the color of cloud2 to blue
  visualizer->setPointCloudRenderingProperties(
      pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 8, "cloud1");
  visualizer->setPointCloudRenderingProperties(
      pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 8, "cloud2");
  visualizer->addCoordinateSystem(5.0);

  while (!visualizer->wasStopped()) {
    visualizer->spinOnce(100);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}