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

//
int main()
{
    int x, y, count;

    FILE *pipein = popen("ffmpeg -loglevel warning -i /usr/stream/cut.avi -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen("ffmpeg -loglevel warning -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1024x576 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 /usr/stream/output.mp4", "w");

    static char bitmap[W*H*3];
    static unsigned char frame[H][W][3] = {0};

    while(1)
    {
        cv::Mat gray_image, image, blured, canny, canny_3d, final_frame, drawing2, mask;

        count = fread( frame, 1, H*W*3, pipein );
        if ( count != H*W*3 ) break;

        memcpy( bitmap, frame, W*H*3 );

        cv::Mat matrix( H, W, CV_8UC3, bitmap );

        cv::cvtColor( matrix, gray_image, CV_BGR2GRAY );
        cv::GaussianBlur( gray_image, blured, cv::Size( 5, 5 ), 0, 0 );
        cv::Canny(blured, canny, 0, 100);
        cv::cvtColor( canny, canny_3d, CV_GRAY2BGR );
        cv::inRange(canny_3d, cv::Scalar(255,255,255), cv::Scalar(255,255,255), mask);
        canny_3d.setTo(cv::Scalar(0, 171, 255), mask);
        cv::GaussianBlur( canny_3d, blured, cv::Size( 5, 5 ), 0, 0 );

        unsigned char* processed_raw_frame = ( unsigned char* )( blured.data );
        fwrite(processed_raw_frame, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    return 0;
}
