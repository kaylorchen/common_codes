//
// Created by kaylor on 1/18/24.
//

#include "fisheye_camera.h"

FisheyeCamera::FisheyeCamera(uint16_t index, cv::InputArray K, cv::InputArray D)
    : capture_(index, cv::CAP_V4L2), size_(1920, 1080) {
  KAYLORDUT_LOG_INFO("Instantiate a FisheyeCamera object");
  // 这里使用V4L2捕获，因为使用默认的捕获不可以设置捕获的模式和帧率
  if (!capture_.isOpened()) {
    KAYLORDUT_LOG_ERROR("Error opening video stream or file");
    exit(EXIT_FAILURE);
  }
  capture_.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
  capture_.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
  capture_.set(cv::CAP_PROP_FPS, 30);
  capture_.set(cv::CAP_PROP_FOURCC,
               cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  // 检查是否成功设置格式
  int fourcc = capture_.get(cv::CAP_PROP_FOURCC);
  if (fourcc != cv::VideoWriter::fourcc('M', 'J', 'P', 'G')) {
    KAYLORDUT_LOG_WARN("Set video format failed");
  }
  cv::Mat newK = cv::getOptimalNewCameraMatrix(K, D, size_, 1, size_, 0);
  cv::fisheye::initUndistortRectifyMap(K, D, cv::Mat(), newK, size_, CV_32FC1,
                                       map1_, map2_);
}

FisheyeCamera::~FisheyeCamera() {
  KAYLORDUT_LOG_INFO("Release sources.")
  capture_.release();
  cv::destroyAllWindows();
}

void FisheyeCamera::Refresh() {
  cv::Mat frame;
  cv::Mat undistortedImage;
  while (true) {
    capture_ >> frame;
    if (frame.empty()) {
      break;
    }
    cv::remap(frame, undistortedImage, map1_, map2_, cv::INTER_LINEAR);
    cv::imshow("Fisheye camera", undistortedImage);
    if (cv::waitKey(1) >= 0) {
      break;
    }
  }
}