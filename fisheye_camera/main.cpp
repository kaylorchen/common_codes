#include "fisheye_camera.h"
int main() {
  // 定义相机内参矩阵K
  cv::Mat K =
      (cv::Mat_<double>(3, 3) << 518.5206194361012, 0.0, 932.0926791943779, 0.0,
       518.0241546073428, 507.22695301062527, 0.0, 0.0, 1.0);

  // 定义畸变系数矩阵D
  cv::Mat D =
      (cv::Mat_<double>(4, 1) << -0.09556402717747697, 0.012374049436718767,
       -0.010465758469831311, 0.0033159128053917544);
  FisheyeCamera fisheye_camera(0, K, D);
//  fisheye_camera.Refresh();
  cv::imwrite("test.jpg", *fisheye_camera.GetRgbFrame(640));
  return 0;
}