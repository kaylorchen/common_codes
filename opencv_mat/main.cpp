#include <iostream>
#include "opencv4/opencv2/opencv.hpp"
#include "kaylordut/log/logger.h"
#include "string.h"

int number = 10;
bool areMatricesEqual(const cv::Mat& mat1, const cv::Mat& mat2) {
    int nonZeroCount = cv::countNonZero(mat1 != mat2);
    return nonZeroCount == 0;
}

int main() {
    KAYLORDUT_LOG_INFO("Opencv Test");
    cv::Mat A = cv::Mat(cv::Size(3000, 2000), CV_8U);
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            A.at<uint8_t>(i, j) = i * A.cols + j + 1 ;
        }
    }
    cv::Mat B;
    KAYLORDUT_TIME_COST_INFO("push back",
                             for (int i = 0; i < number; ++i) {
                                 B.push_back(A);
                             }
    );
//    std::cout << B << std::endl;
    cv::Mat C;
    KAYLORDUT_TIME_COST_INFO("push back transpose",
                             for (int i = 0; i < number; ++i) {
                                 C.push_back(A.t());
                             }
                                     C = C.t();
    );


    cv::Mat D = cv::Mat(cv::Size(A.cols * 10, A.rows), A.type());
    std::cout << "D element size is " << D.elemSize() << std::endl;
    uint8_t *ptr = D.ptr();
    int offset = A.cols * 10;
    int element_size = A.elemSize();
    KAYLORDUT_TIME_COST_INFO("memcpy",
                             for (int i = 0; i < 10; ++i) {
                                 for (int j = 0; j < A.rows; ++j) {
                                     auto *p = ptr + offset * j * element_size + i * A.cols * element_size;
                                     uint8_t *A_ptr = A.ptr() + j * element_size * A.cols;
                                     memcpy(p, A_ptr, element_size * A.cols);
                                 }
                             }
    );


    std::cout << "A size is " << A.size << std::endl;
    std::cout << "B size is " << B.size << std::endl;
    std::cout << "C size is " << C.size << std::endl;
    std::cout << "D size is " << D.size << std::endl;
    if (areMatricesEqual(C ,D)) {
        std::cout << "The two matrices are absolutely equal." << std::endl;
    } else {
        std::cout << "The two matrices are not absolutely equal." << std::endl;
    }
//    std::cout << A << std::endl;
//    std::cout << C << std::endl;
//    std::cout << D << std::endl;
    return 0;
}
