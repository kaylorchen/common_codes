//
// Created by kaylor on 5/9/24.
//
#include "opencv2/opencv.hpp"
#include "kaylordut/log/logger.h"
#include "iostream"
int main(int argc, char *argv[]) {
  KAYLORDUT_LOG_INFO("Starting");
  // 使用cv::Mat定义矩阵A和B
  cv::Mat A = (cv::Mat_<double>(4, 3) << 1, 2,
      3, 4,
      5, 6, 7 , 8, 9, 10, 11, 12);
  cv::Mat B = (cv::Mat_<double>(3, 5) << 1, 2, 3,
      4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

  std::cout << A << "\n" << B << std::endl;
  auto C  = A*B;
  std::cout << C << std::endl;

  auto D = A.rowRange(1, 1 + 2 )*B.colRange(2, 2 + 2);
  std::cout << D << std::endl;

  cv::Mat a = cv::Mat::zeros(2 , 3, CV_32F);
  cv::Mat b;
  for (int i = 0; i < 3; ++i) {
    b.push_back(a);
  }
  std::cout << b << "\n" << a << std::endl;

  return 0;
}