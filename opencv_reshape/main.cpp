#include <opencv2/opencv.hpp>
#include "iostream"

using namespace cv;

int main() {
  // 创建一个单通道的图像，大小为100x100
  Mat gray_image = Mat::ones(6, 10, CV_8UC1); // 单通道，灰度值128
  for (int i = 0; i < gray_image.rows; ++i) {
    for (int j = 0; j < gray_image.cols; ++j) {
      gray_image.at<uchar>(i, j) = i * gray_image.cols + j;
    }
  }

  std::cout << gray_image << std::endl;
//  gray_image = gray_image.t();
  // 将单通道图像转换为三通道图像
  Mat color_image = gray_image.reshape(3, {20, 20} );

  std::cout << color_image << std::endl;

  // 验证输出
  std::cout << "Original dimensions: " << gray_image.size() << ", channels: " << gray_image.channels() << std::endl;
  std::cout << "New dimensions: " << color_image.size() << ", channels: " << color_image.channels() << std::endl;

  return 0;
}