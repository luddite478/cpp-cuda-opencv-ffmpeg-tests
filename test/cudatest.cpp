// g++ -o app app.cpp `pkg-config opencv --cflags --libs`
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"

using namespace cv;

int main (int argc, char* argv[])
{
    try
    {
        cv::Mat src_host = cv::imread("file.png", cv::IMREAD_GRAYSCALE);
        cv::cuda::GpuMat dst, src;
        src.upload(src_host);
        // cv::cuda::threshold(src, dst, 128.0, 255.0, cv::THRESH_BINARY);
        cv::cuda::cvtColor(src, dst, CV_GRAY2BGR);
        cv::Mat result_host(dst);
        cv::imwrite( "./Gray_Image.jpg", result_host );

    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
