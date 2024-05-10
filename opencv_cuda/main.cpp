#include <opencv2/core.hpp>
#include <iostream>
#include "opencv2/cudaarithm.hpp"
#include "kaylordut/log/logger.h"
#include <cublas_v2.h>
#include <cuda_runtime.h>
#include "opencv2/opencv.hpp"

// 错误检查宏
#define CHECK_CUDA(call) \
  if((call) != cudaSuccess) { \
    std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__ << " code=" << (call) << std::endl; \
    exit(1); \
  }

#define CHECK_CUBLAS(call) \
  if((call) != CUBLAS_STATUS_SUCCESS) { \
    std::cerr << "CUBLAS error at " << __FILE__ << ":" << __LINE__ << std::endl; \
    exit(1); \
  }

cv::Mat gpuMatrixMultiply(cv::Mat& A, cv::Mat& B) {
  if (A.cols != B.rows) {
    throw std::runtime_error("Matrix dimensions must agree.");
  }
  int m = A.rows, k = A.cols, n = B.cols;
  cv::Mat C = cv::Mat::zeros(m, n, CV_32FC1);

  // 创建 cuBLAS 上下文
  cublasHandle_t handle;
  CHECK_CUBLAS(cublasCreate(&handle));

  float* d_A;
  float* d_B;
  float* d_C;
  CHECK_CUDA(cudaMalloc(&d_A, sizeof(float) * m * k));
  CHECK_CUDA(cudaMalloc(&d_B, sizeof(float) * k * n));
  CHECK_CUDA(cudaMalloc(&d_C, sizeof(float) * m * n));

  CHECK_CUDA(cudaMemcpy(d_A, A.ptr(), sizeof(float) * m * k, cudaMemcpyHostToDevice));
  CHECK_CUDA(cudaMemcpy(d_B, B.ptr(), sizeof(float) * k * n, cudaMemcpyHostToDevice));

  float alpha = 1.0f, beta = 0.0f;
  CHECK_CUBLAS(cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                           n, m, k, &alpha, d_B, n, d_A, k, &beta, d_C, n));

  CHECK_CUDA(cudaMemcpy(C.ptr(), d_C, sizeof(float) * m * n, cudaMemcpyDeviceToHost));

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
  cublasDestroy(handle);

  return C;
}

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
    int size = 100;
    cv::Mat A_host(10, 32, CV_32F), B_host(32, 409600, CV_32F);
    cv::randu(A_host, -1, 1);
    cv::randu(B_host, -1, 1);
    cv::Mat C;
  KAYLORDUT_TIME_COST_INFO("CUDA",
    cv::cuda::gemm(A_host, B_host, 1.0,cv::noArray(), 0.0, C, 0);
    C = C.t();
  );
  cv::Mat D;
  KAYLORDUT_TIME_COST_INFO("CV",
  D = A_host*B_host;
  D = D.t();
  );

  cv::Mat E;
  KAYLORDUT_TIME_COST_INFO("CV Gemm",
                           cv::gemm(A_host, B_host, 1.0,cv::Mat(), 0.0, E);
  E = E.t();
  );

  cv::Mat F;
  KAYLORDUT_TIME_COST_INFO("For loop",
                           ( F = matmul<float, 1>(A_host, B_host));
                           F= F.t();
  );
//  cv::Mat H;
//  KAYLORDUT_TIME_COST_INFO("For loop_",
//                           ( H = matmul_<float, 1>(A_host, B_host))
//  );

  cv::Mat G;
  KAYLORDUT_TIME_COST_INFO("CUBLAS",
                           G = gpuMatrixMultiply(A_host, B_host).t();
                           );


//  for (int i = 0; i < D.cols  ; i++){
//    KAYLORDUT_LOG_INFO("{} {} {}", C.at<float>(i), F.at<float>(i), G.at<float>(i));
//  }
//
//  std::cout << C << std::endl;
//  std::cout << D << std::endl;


  return 0;
}