#include <opencv2/core.hpp>
#include <iostream>
#include "opencv2/cudaarithm.hpp"
#include "kaylordut/log/logger.h"

template<typename T, int channels>
cv::Mat matmul(const cv::Mat &a, const cv::Mat &b, bool result_transpose = false) {
  if (a.cols != b.rows || a.channels() != b.channels() || a.type() != b.type()) {
    exit(EXIT_FAILURE);
  }
  cv::Mat c;
  if (result_transpose) {
    c = cv::Mat::zeros(b.cols, a.rows, a.type());
  } else {
    c = cv::Mat::zeros(a.rows, b.cols, a.type());
  }
  int row = 0;
  int col = 0;
  for (int i = 0; i < a.rows; ++i) {
    for (int k = 0; k < a.cols; ++k) {
      for (int j = 0; j < b.cols; ++j) {
        if (result_transpose) {
          row = j;
          col = i;
        } else {
          row = i;
          col = j;
        }
        auto &c_pixel = c.at<cv::Vec<T, channels>>(row, col);
        auto &a_pixel = a.at<cv::Vec<T, channels>>(i, k);
        auto &b_pixel = b.at<cv::Vec<T, channels>>(k, j);
        for (int l = 0; l < channels; ++l) {
          c_pixel[l] += (a_pixel[l] * b_pixel[l]);
        }
      }
    }
  }
  return c;
}

template<typename T, int channels>
cv::Mat matmul_(const cv::Mat &a, const cv::Mat &b, bool result_transpose = false) {
  if (a.cols != b.rows || a.channels() != b.channels() || a.type() != b.type()) {
    exit(EXIT_FAILURE);
  }
  cv::Mat c;
  if (result_transpose) {
    c = cv::Mat::zeros(b.cols, a.rows, a.type());
  } else {
    c = cv::Mat::zeros(a.rows, b.cols, a.type());
  }
  int row = 0;
  int col = 0;
  for (int i = 0; i < a.rows; ++i) {
    for (int j = 0; j < b.cols; ++j) {
    for (int k = 0; k < a.cols; ++k) {
        if (result_transpose) {
          row = j;
          col = i;
        } else {
          row = i;
          col = j;
        }
        auto &c_pixel = c.at<cv::Vec<T, channels>>(row, col);
        auto &a_pixel = a.at<cv::Vec<T, channels>>(i, k);
        auto &b_pixel = b.at<cv::Vec<T, channels>>(k, j);
        for (int l = 0; l < channels; ++l) {
          c_pixel[l] += (a_pixel[l] * b_pixel[l]);
        }
      }
    }
  }
  return c;
}

int main() {
    // 检查是否有可用的CUDA设备
    if (cv::cuda::getCudaEnabledDeviceCount() == 0) {
      KAYLORDUT_LOG_INFO("CUDA is not enabled");
    }
    cv::cuda::setDevice(0);

    // 创建两个随机的500x500浮点矩阵
    int size = 1500;
    cv::Mat A_host(size, size, CV_32F), B_host(size, size, CV_32F);
    cv::randu(A_host, -1, 1);
    cv::randu(B_host, -1, 1);
    cv::Mat C;
  KAYLORDUT_TIME_COST_INFO("CUDA",
    cv::cuda::gemm(A_host, B_host, 1.0,cv::noArray(), 0.0, C, 0);
  );
  cv::Mat D;
  KAYLORDUT_TIME_COST_INFO("CV",
  D = A_host*B_host;);

  cv::Mat E;
  KAYLORDUT_TIME_COST_INFO("CV Gemm",
                           cv::gemm(A_host, B_host, 1.0,cv::Mat(), 0.0, E);
  );

  cv::Mat F;
  KAYLORDUT_TIME_COST_INFO("For loop",
                           ( F = matmul<float, 1>(A_host, B_host))
  );
  cv::Mat H;
  KAYLORDUT_TIME_COST_INFO("For loop_",
                           ( H = matmul_<float, 1>(A_host, B_host))
  );

  for (int i = 0; i < D.cols  ; i++){
    KAYLORDUT_LOG_INFO("{} {} {}", C.at<float>(i), F.at<float>(i), H.at<float>(i));
  }

//  std::cout << C << std::endl;
//  std::cout << D << std::endl;


  return 0;
}