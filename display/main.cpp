#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
  // 读取要显示的图片
  cv::Mat image = cv::imread("../test.png");
  if (image.empty()) {
    std::cerr << "无法加载图片" << std::endl;
    return -1;
  }
  cv::resize(image, image, cv::Size(1920, 1080));

  // 创建一个窗口
  cv::namedWindow("FullScreen Image", cv::WINDOW_NORMAL);

  // 设置窗口为全屏
  cv::setWindowProperty("FullScreen Image", cv::WND_PROP_FULLSCREEN,
                        cv::WINDOW_FULLSCREEN);

  // 显示图片
  cv::imshow("FullScreen Image", image);

  // 等待按键按下以关闭窗口
  cv::waitKey(0);

  // 销毁窗口
  cv::destroyWindow("FullScreen Image");

  return 0;
}
