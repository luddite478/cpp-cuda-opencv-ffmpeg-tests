// g++ -o app app.cpp `pkg-config opencv --cflags --libs`
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"

using namespace cv;

void blue_edges(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask)
{
    try
    {
        cv::cuda::GpuMat dst, src;
        src.upload(matrix);
        // cv::cuda::threshold(src, dst, 128.0, 255.0, cv::THRESH_BINARY);
        cv::cuda::cvtColor(src, dst, CV_GRAY2BGR);
        cv::Mat matrix(dst);

    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
}

struct blue_edges_filter {
  blue_edges_filter(int width, int height)
  : gray_image(width, height, CV_8UC1),
    blured(width, height, CV_8UC1),
    canny(width, height, CV_8UC1),
    canny_3d(width, height, CV_8UC3),
    in_range_mask(width, height, CV_8UC3)
  {  }

  int operator()(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask) {

    cv::bitwise_and(matrix, mask, internal_mask_matrix);
    cv::bitwise_and(matrix, inverted_mask, external_mask_matrix);

    cv::cvtColor( matrix, gray_image, CV_BGR2GRAY );
    cv::GaussianBlur( gray_image, blured, cv::Size( 5, 5 ), 0, 0 );
    cv::Canny(blured, canny, 0, 100);
    cv::cvtColor( canny, canny_3d, CV_GRAY2BGR );
    cv::inRange(canny_3d, cv::Scalar(255,255,255), cv::Scalar(255,255,255), in_range_mask);
    canny_3d.setTo(cv::Scalar(0, 171, 255), in_range_mask);
    cv::GaussianBlur( canny_3d, matrix, cv::Size( 5, 5 ), 0, 0 );
    cv::bitwise_and(matrix, mask, matrix);

    cv::cvtColor( external_mask_matrix, external_mask_matrix, CV_BGR2XYZ );

    cv::add(matrix, external_mask_matrix, matrix);
  }

  private:
    cv::Mat gray_image, blured, canny, canny_3d, in_range_mask;
    cv::Mat external_mask_matrix, internal_mask_matrix;
};


// void cuda_apply_blue_edgess(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask){
//
//   cv::cuda::GpuMat gpu_mask, gpu_inverted_mask, src;
//   cv::cuda::GpuMat gray_image, blured, canny, canny_3d;
//   cv::cuda::GpuMat external_mask_matrix, internal_mask_matrix;
//
//   cv::Mat in_range_mask;
//
//   src.upload(matrix);
//   gpu_mask.upload(mask);
//   gpu_inverted_mask.upload(inverted_mask);
//
//   cv::cuda::bitwise_and(src, gpu_mask, internal_mask_matrix);
//   cv::cuda::bitwise_and(matrix, inverted_mask, external_mask_matrix);
//
//   cv::cuda::cvtColor( matrix, gray_image, CV_BGR2GRAY );
//   cv::cuda::GaussianBlur( gray_image, blured, cv::Size( 5, 5 ), 0, 0 );
//   cv::cuda::Canny(blured, canny, 0, 100);
//   cv::cuda::cvtColor( canny, canny_3d, CV_GRAY2BGR );
//
//   cv::Mat cuda_res(canny_3d);
//
//   cv::inRange(cuda_res, cv::Scalar(255,255,255), cv::Scalar(255,255,255), in_range_mask);
//
//   canny_3d.setTo(cv::Scalar(0, 171, 255), in_range_mask);
//   cv::GaussianBlur( canny_3d, matrix, cv::Size( 5, 5 ), 0, 0 );
//   cv::bitwise_and(matrix, mask, matrix);
//
// }
