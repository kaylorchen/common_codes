//
// Created by kaylor on 9/16/24.
//
#include "kaylordut/log/logger.h"
#include "opencv2/opencv.hpp"
#include "iostream"

template <typename T, int channels>
cv::Mat matmul1(const cv::Mat &a, const cv::Mat &b,
               bool result_transpose = false) {
  if (a.cols != b.rows || a.channels() != b.channels() ||
      a.type() != b.type()) {
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

template <typename T, int channels>
cv::Mat matmul2(const cv::Mat &a, const cv::Mat &b,
               bool result_transpose = false) {
  if (a.cols != b.rows || a.channels() != b.channels() ||
      a.type() != b.type()) {
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
int A_rows = 200;
int A_cols = 10000;
int B_rows = A_cols;
int B_cols = 300;
int main(int argc, char **argv){
  KAYLORDUT_LOG_INFO("This is matmul test");
  cv::Mat A(A_rows, A_cols, CV_32FC1);
  cv::randu(A, cv::Scalar::all(0), cv::Scalar::all(1));
  cv::Mat B(B_rows, B_cols, CV_32FC1);
  cv::randu(B, cv::Scalar::all(0), cv::Scalar::all(1));
  cv::Mat C;
  cv::Mat D;
  auto func1 = [&](){C = matmul1<float, 1>(A, B);};
  auto func2 = [&](){D = matmul2<float, 1>(A, B);};
  KAYLORDUT_TIME_COST_INFO("Matmul1", func1());
  KAYLORDUT_TIME_COST_INFO("Matmul2", func2());
//  std::cout << C << std::endl << D << std::endl;
  return 0;
}