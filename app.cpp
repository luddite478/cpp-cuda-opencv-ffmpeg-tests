#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>

#define W 1024
#define H 576

// void apply_blue_edgess(cv::Mat& matrix);
// void apply_blue_edgess(cv::Mat& matrix, int mask_h, int mask_w);

void createMask(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask) {
  int mask_h = H * 50/100;
  int mask_w = W ;
  cv::Mat mask_matrix_1d = cv::Mat::zeros(H, W, CV_8U);

  mask_matrix_1d(cv::Rect(0, H * 25/100, mask_w, mask_h)) = 255;

  cv::Mat inverted_mask_1d;
  cv::bitwise_not(mask_matrix_1d, inverted_mask_1d);
  cv::cvtColor( mask_matrix_1d, mask, CV_GRAY2BGR );
  cv::cvtColor( inverted_mask_1d, inverted_mask, CV_GRAY2BGR );
}

void apply_blue_edgess(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask) {

  cv::Mat matrix_with_mask, external_mask_matrix, internal_mask_matrix;
  cv::bitwise_and(matrix, mask, internal_mask_matrix);
  cv::bitwise_and(matrix, inverted_mask, external_mask_matrix);

  cv::Mat gray_image, blured, canny, canny_3d, in_range_mask;
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

int main(int argc, char *argv[])
{
    std::string input_filename = argv[1]; //cut.mp4
    std::string width = argv[2];
    std::string height = argv[3];
    std::string framerate = argv[4]; //25
    const std::string output_filename = "output.mp4";
    const std::string h264_preset = "slow";

    std::string ffmpeg_pipe_in_cmd = "ffmpeg -loglevel warning -vsync 0 -c:v h264_cuvid" +
    " -i " + filename + " -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -";

    std::string ffmpeg_pipe_out_cmd = "ffmpeg -loglevel warning -y -f rawvideo" +
    " -s:v " + width + "x" + height +
    " -r " + framerate +
    " -pix_fmt rgb24" +
    " -i - -c:v h264_nvenc" +
    " -preset " + h264_preset +
    " -cq 10 -bf 2 -g 150 " + output_filename;

    FILE *pipein = popen(ffmpeg_pipe_in_cmd, "r");
    FILE *pipeout = popen(ffmpeg_pipe_out_cmd, "w");

    static char bitmap[W*H*3];
    static unsigned char frame[H][W][3] = {0};

    cv::Mat mask;
    cv::Mat inverted_mask;
    int count;

    while(1)
    {
        count = fread( frame, 1, H*W*3, pipein );
        if ( count != H*W*3 ) break;

        memcpy( bitmap, frame, W*H*3 );

        cv::Mat matrix( H, W, CV_8UC3, bitmap );
        if (mask.empty() == 1){
          createMask(matrix, mask, inverted_mask);
        }

        apply_blue_edgess(matrix, mask, inverted_mask);

        unsigned char* processed_raw_frame = ( unsigned char* )( matrix.data );
        fwrite(processed_raw_frame, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    return 0;
}
