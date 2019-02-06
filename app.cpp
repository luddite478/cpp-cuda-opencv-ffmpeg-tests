#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
// https://batchloaf.wordpress.com/2017/02/12/a-simple-way-to-read-and-write-audio-and-video-files-in-c-using-ffmpeg-part-2-video/
//g++ -Wl,-Bstatic -static-libgcc -std=c++11 /usr/stream/app.cpp -o /usr/stream/app $(pkg-config --cflags --libs -static opencv) -lgfortran -lquadmath

// int main( int argc, char** argv )
// {
//     if( argc != 2)
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
#include <stdio.h>

// Video resolution
#define W 1024
#define H 576


unsigned char frame[H][W][3] = {0};

int main()
{
    int x, y, count;

    FILE *pipein = popen("ffmpeg -loglevel warning -i /usr/stream/cut.avi -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen("ffmpeg -loglevel warning -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1024x576 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 /usr/stream/output.mp4", "w");
    static char bitmap[W*H*3];
    // Process video frames
    while(1)
    {

        cv::Mat gray_image, image;
        count = fread( frame, 1, H*W*3, pipein );

        if ( count != H*W*3 ) break;


        memcpy(bitmap, frame, W*H*3);
        cv::Mat mat( H, W, CV_8UC3, bitmap );

        // std::cout << bits << std::endl;

        // image = Mat(H, W, cv::CV_8UC3, count);

        cv::cvtColor( mat, gray_image, CV_BGR2HSV );

        unsigned char *processed_raw_frame = (unsigned char*)(gray_image.data);

        fwrite(processed_raw_frame, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    return 0;
}
