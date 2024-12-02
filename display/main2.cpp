//
// Created by kaylor on 12/2/24.
//
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>
#include <opencv2/opencv.hpp>
#include <thread>

void createCustomImage(const cv::Size& size, cv::Mat& image) {
  // 创建一个bgra格式的Mat，并填充为全黑
  image = cv::Mat(size, CV_8UC4, cv::Scalar(0, 0, 0, 255));  // 初始值为透明

  // 根据矩阵的大小填充四个不同的区域

  // 左上角区域 - 红色 (B, G, R, A = 0, 0, 255, 255)
  cv::Rect topLeftRect(0, 0, size.width / 2, size.height / 2);
  image(topLeftRect).setTo(cv::Scalar(0, 0, 255, 255));

  // 右上角区域 - 蓝色 (B, G, R, A = 255, 0, 0, 255)
  cv::Rect topRightRect(size.width / 2, 0, size.width - size.width / 2,
                        size.height / 2);
  image(topRightRect).setTo(cv::Scalar(255, 0, 0, 255));

  // 左下角区域 - 绿色 (B, G, R, A = 0, 255, 0, 255)
  cv::Rect bottomLeftRect(0, size.height / 2, size.width / 2,
                          size.height - size.height / 2);
  image(bottomLeftRect).setTo(cv::Scalar(0, 255, 0, 255));

  // 右下角区域 - 白色 (B, G, R, A = 255, 255, 255, 255)
  cv::Rect bottomRightRect(size.width / 2, size.height / 2,
                           size.width - size.width / 2,
                           size.height - size.height / 2);
  image(bottomRightRect).setTo(cv::Scalar(255, 255, 255, 255));
}

int main() {
  // 打开图片
  cv::Mat image = cv::imread("../test.jpg");
  if (image.empty()) {
    std::cerr << "无法加载图片" << std::endl;
    return -1;
  }

  // 打开帧缓冲设备
  int fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    std::cerr << "无法打开帧缓冲设备" << std::endl;
    return -1;
  }

  // 尝试独占锁定帧缓冲设备
  if (flock(fbfd, LOCK_EX | LOCK_NB) == -1) {
    perror("Error locking framebuffer device");
    close(fbfd);
    return -1;
  }

  fb_fix_screeninfo finfo;
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    std::cerr << "无法获取屏幕信息" << std::endl;
    flock(fbfd, LOCK_UN);
    close(fbfd);
    return -1;
  }
  std::cout << "smem_len: " << finfo.smem_len
            << " line_lenght: " << finfo.line_length << std::endl;

  // 获取屏幕信息
  fb_var_screeninfo vinfo;
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    std::cerr << "无法获取屏幕信息" << std::endl;
    flock(fbfd, LOCK_UN);
    close(fbfd);
    return -1;
  }

  // 调整图片大小以匹配屏幕分辨率
  cv::resize(image, image, cv::Size(vinfo.xres, vinfo.yres));

  // 转换图片通道顺序 (BGR -> RGB)
  cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);

  // 计算帧缓冲区大小
  size_t screensize =
      vinfo.yres_virtual * vinfo.xres_virtual * vinfo.bits_per_pixel / 8;

  // 将图像写入帧缓冲区
  char* fbp =
      (char*)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((long)fbp == -1) {
    std::cerr << "映射帧缓冲区失败" << std::endl;
    flock(fbfd, LOCK_UN);
    close(fbfd);
    return -1;
  }
  auto dst = fbp;
  auto src = image.data;
  for (int i = 0; i < image.rows; i++) {
    std::memcpy(dst, src, image.cols * image.elemSize());
    dst += finfo.line_length;
    src += image.cols * image.elemSize();
  }

  // 释放资源
  munmap(fbp, finfo.smem_len);
  flock(fbfd, LOCK_UN);
  close(fbfd);

  return 0;
}
