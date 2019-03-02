#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
// https://batchloaf.wordpress.com/2017/02/12/a-simple-way-to-read-and-write-audio-and-video-files-in-c-using-ffmpeg-part-2-video/
//g++ -Wl,-Bstatic -static-libgcc -std=c++11 /usr/stream/app.cpp -o /usr/stream/app $(pkg-config --cflags --libs -static opencv) -lgfortran -lquadmath

// int main( int argc, char** argv )
// {
      // std::cout << cv::getBuildInformation() << std::endl;
//    if( argc != 2)
//     {
//      std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
//      return -1;
//     }
//
//     cv::Mat image, gray_image;
//     image = cv::imread(argv[1], cv::IMREAD_COLOR); // Read the file
//
//
//     if(! image.data ) // Check for invalid input
//     {
//         std::cout << "Could not open or find the image" << std::endl ;
//         return -1;
//     }
//
//     cv::cvtColor( image, gray_image, CV_BGR2GRAY );
//     cv::imwrite( "./Gray_Image.jpg", gray_image );
//
//     return 0;
// }


// Video resolution
#define W 1024
#define H 576
//
//

void apply_blue_edgess(cv::Mat& matrix);
void apply_blue_edgess(cv::Mat& matrix, int mask_h, int mask_w);

// void apply_blue_edgess(cv::Mat& matrix) {
//
//   cv::Mat gray_image, image, blured, canny, canny_3d, final_frame, drawing2, in_range_mask;
//
//   cv::cvtColor( matrix, gray_image, CV_BGR2GRAY );
//   cv::GaussianBlur( gray_image, blured, cv::Size( 5, 5 ), 0, 0 );
//   cv::Canny(blured, canny, 0, 100);
//   cv::cvtColor( canny, canny_3d, CV_GRAY2BGR );
//   cv::inRange(canny_3d, cv::Scalar(255,255,255), cv::Scalar(255,255,255), in_range_mask);
//   canny_3d.setTo(cv::Scalar(0, 171, 255), in_range_mask);
//   cv::GaussianBlur( canny_3d, matrix, cv::Size( 5, 5 ), 0, 0 );
// }

// void apply_blue_edgess(cv::Mat& matrix, int mask_h, int mask_w) {
//
//   cv::Mat hsv, gray_image, image, blured, canny, canny_3d, final_frame, drawing2, in_range_mask, outputMat, inverted_mask, outputMat_3d;
//
//   mask_h = H * 80/100;
//   mask_w = W * 80/100;
//   cv::Mat mask = cv::Mat::zeros(H, W, CV_8U);
//   mask(cv::Rect(W * 10/100, H * 10/100, mask_w,mask_h)) = 0;
  // cv::bitwise_not(mask, inverted_mask);
  //
  // cv::cvtColor( matrix, hsv, CV_BGR2HSV );
  //
  // cv::cvtColor( mask, outputMat_3d, CV_GRAY2BGR );
  //
  // cv::add(image, outputMat_3d, matrix);
  // Mat imageDest = cvCreateMat(480, 640, CV_8UC3);

  // cv::cvtColor( matrix, gray_image, CV_BGR2GRAY );
  // gray_image.copyTo(outputMat, mask);
  // cv::bitwise_and(matrix, matrix, mask, outputMat);
  // cv::bitwise_and(matrix, matrix, inverted_mask, outputMat);
  // cv::cvtColor( outputMat, outputMat_3d, CV_GRAY2BGR );


  // cv::add(matrix, outputMat_3d, final_frame);
  // cv::GaussianBlur( outputMat, blured, cv::Size( 5, 5 ), 0, 0 );
  // cv::Canny(blured, canny, 0, 100);
  // cv::cvtColor( inverted_mask, matrix, CV_GRAY2BGR );


  // cv::inRange(canny_3d, cv::Scalar(255,255,255), cv::Scalar(255,255,255), in_range_mask);
  // canny_3d.setTo(cv::Scalar(0, 171, 255), in_range_mask);
  // cv::GaussianBlur( canny_3d, matrix, cv::Size( 5, 5 ), 0, 0 );
// }

// void createMask(cv::Mat& matrix, cv::Mat& mask) {
  // cv::Mat hsv, mask_3d;
  // int mask_h = H * 80/100;
  // int mask_w = W * 80/100;
  // mask = cv::Mat::zeros(H, W, CV_8U);
  // mask(cv::Rect(W * 10/100, H * 10/100, mask_w,mask_h)) = 0;
  // cv::bitwise_not(mask, inverted_mask);
  // cv::cvtColor( matrix, hsv, CV_BGR2HSV );
  // cv::cvtColor( mask, mask_3d, CV_GRAY2BGR );
// }


// void apply_blue_edgess(cv::Mat& matrix) {
//   Rect r(10, 10, 100, 100);
//   Mat imgROI = img(r);
// }
//
int main()
{
    int x, y, count;

    FILE *pipein = popen("ffmpeg -loglevel warning -i /usr/stream/cut.avi -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen("ffmpeg -loglevel warning -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1024x576 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 /usr/stream/output.mp4", "w");

    static char bitmap[W*H*3];
    static unsigned char frame[H][W][3] = {0};

    // cv::Mat mask;
    // cv::Mat inverted_mask;

    while(1)
    {
        count = fread( frame, 1, H*W*3, pipein );
        if ( count != H*W*3 ) break;
        // if ( mask.size() == 0 && inverted_mask.size() == 0) {
        // createMask(frame);
        // }

        memcpy( bitmap, frame, W*H*3 );
        cv::Mat matrix( H, W, CV_8UC3, bitmap );
        // apply_blue_edgess(matrix, 800, 400);

        unsigned char* processed_raw_frame = ( unsigned char* )( matrix.data );
        fwrite(matrix, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    return 0;
}
