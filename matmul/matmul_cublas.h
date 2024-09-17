//
// Created by kaylor on 5/9/24.
//

#pragma once

#include <cublas_v2.h>
#include <cuda_runtime.h>

#include "opencv2/opencv.hpp"
#include "kaylordut//log/logger.h"

// 错误检查宏
#define CHECK_CUDA(call)                                                       \
  if ((call) != cudaSuccess) {                                                 \
    KAYLORDUT_LOG_ERROR("CUDA error at {}:{} code = {}", __FILE__, __LINE__,    \
                       (call));                                                \
    exit(1);                                                                   \
  }

#define CHECK_CUBLAS(call)                                                     \
  if ((call) != CUBLAS_STATUS_SUCCESS) {                                       \
    std::cerr << "CUBLAS error at " << __FILE__ << ":" << __LINE__             \
              << std::endl;                                                    \
    KAYLORDUT_LOG_ERROR("CUBLAS error at {}:{}", __FILE__, __LINE__);           \
    exit(1);                                                                   \
  }

cv::Mat gpuMatrixMultiply(cv::Mat &A, cv::Mat &B) {
  if (A.cols != B.rows) {
    throw std::runtime_error("Matrix dimensions must agree.");
  }
  int m = A.rows, k = A.cols, n = B.cols;
  cv::Mat C = cv::Mat::zeros(m, n, CV_32FC1);

  // 创建 cuBLAS 上下文
  cublasHandle_t handle;
  CHECK_CUBLAS(cublasCreate(&handle));

  float *d_A;
  float *d_B;
  float *d_C;
  CHECK_CUDA(cudaMalloc(&d_A, sizeof(float) * m * k));
  CHECK_CUDA(cudaMalloc(&d_B, sizeof(float) * k * n));
  CHECK_CUDA(cudaMalloc(&d_C, sizeof(float) * m * n));

  CHECK_CUDA(
      cudaMemcpy(d_A, A.ptr(), sizeof(float) * m * k, cudaMemcpyHostToDevice));
  CHECK_CUDA(
      cudaMemcpy(d_B, B.ptr(), sizeof(float) * k * n, cudaMemcpyHostToDevice));

  float alpha = 1.0f, beta = 0.0f;
  CHECK_CUBLAS(cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, m, k, &alpha,
                           d_B, n, d_A, k, &beta, d_C, n));

  CHECK_CUDA(
      cudaMemcpy(C.ptr(), d_C, sizeof(float) * m * n, cudaMemcpyDeviceToHost));

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
  cublasDestroy(handle);

  return C;
}
