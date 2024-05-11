//
// Created by kaylor on 5/10/24.
//
#include "kaylordut/log/logger.h"
#include "opencv2/dnn.hpp"
#include "opencv2/opencv.hpp"
#include "iostream"
#include "stdio.h"

using namespace std;

const int width = 6400;
const int height = 6400;

uint64_t PopulateData(const cv::Mat &data, float *dst) {
  std::cout << sizeof(*dst) << " " << sizeof(dst) << std::endl;
  if (data.channels() != 3 || data.type() != CV_8UC3) {
    KAYLORDUT_LOG_ERROR("Invalid data");
    return 0;
  }
//  auto result = std::make_unique<float[]>(data.total() * data.channels());
  auto *R = dst;
  auto *G = dst + data.total();
  auto *B = dst + data.total() * 2;
  auto *src = data.ptr();
//  for (int i = 0; i < data.total()*data.channels(); i +=3) {
//    *B = *(src + i);
//    B++;
//    *G = *(src + i + 1);
//    G++;
//    *R = *(src + i + 2);
//    R++;
//  }
//  for (int i = 0; i < data.total() * data.channels(); ++i) {
//    *dst = (*dst)/255.0f;
//    dst++;
//  }
//  for (int i = 0; i < data.total()*data.channels(); i +=3) {
//    *B = *(src + i)/255.0f;
//    B++;
//    *G = *(src + i + 1)/255.0f;
//    G++;
//    *R = *(src + i + 2)/255.0f;
//    R++;
//  }
//  for (int i = 0; i < data.rows; ++i) {
//    for (int j = 0; j < data.cols; ++j) {
//      // Mat 的数据是BGR
//      *B = data.at<cv::Vec3b>(i, j)[0]/255.0f;
//      B++;
//      *G = data.at<cv::Vec3b>(i, j)[1]/255.0f;
//      G++;
//      *R = data.at<cv::Vec3b>(i, j)[2]/255.0f;
//      R++;
//    }
//  }
  for (int i = 0; i < data.rows; ++i) {
    for (int j = 0; j < data.cols; ++j) {
      // Mat 的数据是BGR
      auto tmp = data.ptr<cv::Vec3b>(i)[j];
      *B = tmp[0] / 255.0f;
      B++;
      *G = tmp[1] / 255.0f;
      G++;
      *R = tmp[2] / 255.0f;
      R++;
    }
  }
  // 返回填充的字节数
  return data.total() * data.elemSize();
}

int main(int argc, char *argv[]) {

  cv::Mat A = cv::Mat(cv::Size(width, height), CV_8UC3);
  std::cout << "A type is " << A.type() << ", CV_8UC3 is " << CV_8UC3 << "\n";
  std::cout << A.elemSize() << "\n";
  std::cout << A.total() << "\n";
  std::cout << A.channels() << std::endl;
  cv::randu(A,cv::Scalar(0,0,0), cv::Scalar (255,255,255));
//  for (int i = 0; i < A.rows; i++) {
//    for (int j = 0; j < A.cols; ++j) {
//      A.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 128, 255);
//    }
//  }
//  std::cout << A << std::endl;

  cv::Mat B;
  KAYLORDUT_TIME_COST_INFO(
      "cv::dnn::blobFromImage",
      cv::dnn::blobFromImage(A, B, 1 / 255.0f, cv::Size(), cv::Scalar(0, 0, 0), true, false, CV_32F);
  );
//  cout << "Blob shape: " << B.size[0] << "x" << B.size[1] << "x" << B.size[2] << "x" << B.size[3] << endl;
//  int N = B.size[0];  // 批大小
//  int C = B.size[1];  // 通道数
//  int H = B.size[2];  // 高度
//  int W = B.size[3];  // 宽度
//  cout << "Partial data from blob:" << endl;
//  for (int n = 0; n < N; n++) {
//    cout << "Batch index: " << n << endl;
//    for (int c = 0; c < C; c++) {
//      cout << "Channel index: " << c << endl;
//      // 获取对应层的数据指针
//      const float* data = B.ptr<float>(n, c);
//      for (int h = 0; h < H; h++) {
//        for (int w = 0; w < W; w++) {
//          // 计算对应的索引
//          float value = data[h * W + w];
//          cout << value << " ";
//        }
//        cout << endl;
//      }
//    }
//  }
  auto *ptr = (float *) (B.ptr());
//  for (int i = 0; i < A.rows; ++i) {
//    for (int j = 0; j < A.cols; ++j) {
//      for (int k = 0; k < 3; ++k) {
////        std::cout << *ptr++ << " ";
//        printf("%.2f ", *ptr++);
//      }
//    }
//    std::cout << std::endl;
//  }
//

  auto result = std::make_unique<float[]>(A.total() * A.channels());
  KAYLORDUT_TIME_COST_INFO(
      "Populate data",
      PopulateData(A, result.get());
  );
//  ptr = result.get();
//  for (int i = 0; i < A.rows; ++i) {
//    for (int j = 0; j < A.cols; ++j) {
//      for (int k = 0; k < 3; ++k) {
////        std::cout << *ptr++ << " ";
//        printf("%.2f ", *ptr++);
//      }
//    }
//    std::cout << std::endl;
//  }


  return 0;
}