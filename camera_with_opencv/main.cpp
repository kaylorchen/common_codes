#include <iostream>
#include "yaml-cpp/yaml.h"
#include "vector"
#include "sstream"
#include "chrono"
#include "iomanip"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
void capture_and_save_video(const std::string& device_name, const std::string& save_name) {
  cv::VideoCapture cap(device_name);  // 使用设备名称打开视频捕获设备

  if (!cap.isOpened()) {
    std::cerr << "Error: Unable to open camera device " << device_name << std::endl;
    return;
  }

  // 获取捕获的帧率和帧尺寸
  double fps = cap.get(cv::CAP_PROP_FPS);
  int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
  int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

  // 初始化VideoWriter对象
  cv::VideoWriter writer(save_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight));

  if (!writer.isOpened()) {
    std::cerr << "Error: Unable to open video writer" << std::endl;
    return;
  }

  cv::Mat frame;
  int frameCount = 0;
  const int maxFrames = 150;  // 设置最大帧数限制

  while (frameCount < maxFrames) {
    if (!cap.read(frame)) {  // 从摄像头读取一帧图像到frame中
      std::cerr << "Error: Unable to capture image from camera device " << device_name << std::endl;
      break;
    }

    writer.write(frame);  // 将帧写入视频文件中
    frameCount++;
  }

  cap.release(); // 释放VideoCapture对象
  writer.release(); // 释放VideoWriter对象
}
void capture_and_save_image(const std::string &device_name, const std::string &save_name) {
  // 使用设备名打开视频捕获设备
  cv::VideoCapture cap(device_name);

  if (!cap.isOpened()) {
    std::cerr << "Error: Unable to open camera device " << device_name << std::endl;
    return;
  }

  cv::Mat frame;
  int frameCount = 0;
  const int targetFrame = 10;  // 我们想舍弃前10帧，保存第11帧

  while (frameCount <= targetFrame) {
    if (!cap.read(frame)) {  // 从摄像头读取一帧图像到frame中
      std::cerr << "Error: Unable to capture image from camera device " << device_name << std::endl;
      return;
    }

    if (frameCount == targetFrame) {
      // 当达到目标帧时, 保存图像
      if (!cv::imwrite(save_name, frame)) {
        std::cerr << "Error: Failed to save the image to " << save_name << std::endl;
        return;
      }
      std::cout << "Image saved successfully to " << save_name << std::endl;
    }
    frameCount++;
  }

  cap.release(); // 释放VideoCapture对象
}

// 函数用于获取当前时间并返回格式化的字符串
std::string GetCurrentDateTime() {
  // 获取当前时间点
  auto now = std::chrono::system_clock::now();
  // 转换为time_t类型
  auto now_c = std::chrono::system_clock::to_time_t(now);

  // 格式化时间
  std::ostringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S");

  return ss.str();
}



int main(int argc, char *argv[]) {
  std::string config_filename(argv[1]);
  YAML::Node config = YAML::LoadFile(config_filename);
  std::vector<std::string> devices;
  if (config["devices"] && config["devices"].IsSequence()){
    for (int i = 0; i < config["devices"].size(); ++i) {
      devices.emplace_back(config["devices"][i].as<std::string>());
    }
  }
  auto current_time = GetCurrentDateTime();
  std::cout << "current time is " << current_time << std::endl;
  for (int i = 0; i < devices.size(); ++i) {
    std::cout << "processing device " << devices[i] << std::endl;
//    capture_and_save_image(devices.at(i), current_time + "_" + std::to_string(i) +".jpg");
    capture_and_save_video(devices.at(i), current_time + "_" + std::to_string(i) +".mp4");
  }

  return 0;
}

