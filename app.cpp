#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include "blue_edges.cpp"
#define BENCH

#ifdef BENCH
  #include <sys/time.h>
  typedef unsigned long long timestamp_t;

  static timestamp_t
  get_timestamp ()
  {
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
  }
#endif

void createMask(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask, const int H, const int W) {
  const int mask_h = H * 50/100;
  const int mask_w = W ;
  cv::Mat mask_matrix_1d = cv::Mat::zeros(H, W, CV_8U);

  mask_matrix_1d(cv::Rect(0, H * 25/100, mask_w, mask_h)) = 255;

  cv::Mat inverted_mask_1d;
  cv::bitwise_not(mask_matrix_1d, inverted_mask_1d);
  cv::cvtColor( mask_matrix_1d, mask, CV_GRAY2BGR );
  cv::cvtColor( inverted_mask_1d, inverted_mask, CV_GRAY2BGR );
}

void black_and_white_filter(cv::Mat& matrix) {

  cv::cvtColor( matrix, matrix, CV_BGR2GRAY );
  cv::cvtColor( matrix, matrix, CV_GRAY2BGR );
}


int main(int argc, char *argv[])
{
    #ifdef BENCH
      timestamp_t t0 = get_timestamp();
    #endif

    if(argc < 5) return 1;

    std::string input_filename = argv[1]; //cut.mp4
    std::string width = argv[2];
    std::string height = argv[3];
    std::string framerate = argv[4];
    std::string ff_decoder = argv[5];
    std::string ff_encoder = argv[6];
    std::string ff_loglvl = argv[7];
    const std::string output_filename = "video_tmp.mp4";
    const std::string h264_preset = "slow";


    std::string ffmpeg_pipe_in_cmd, ffmpeg_pipe_out_cmd;
    ffmpeg_pipe_in_cmd += std::string("ffmpeg -loglevel ") +  std::string(ff_loglvl)
                       +  std::string(" -c:v ") + std::string(ff_decoder)
                       +  std::string(" -i ") + std::string(input_filename)
                       +  std::string(" -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -");

    ffmpeg_pipe_out_cmd += std::string("ffmpeg -loglevel quiet -y -f rawvideo")
                        +  std::string(" -s:v ") + std::string(width) + std::string("x") + std::string(height)
                        +  std::string(" -r ") + std::string(framerate)
                        +  std::string(" -pix_fmt ") + std::string("rgb24")
                        +  std::string(" -i - ")
                        +  std::string(" -c:v ") + std::string(ff_encoder)
                        +  std::string(" -preset ") + std::string(h264_preset)
                        +  std::string(" -qp 1 ")
                        +  std::string(output_filename);
//                        +  std::string(" -avoid_negative_ts make_zero -fflags +genpts ")

//                        +  std::string(" -cq 1 -bf 2 -g 150 ")

    FILE *pipein = popen(ffmpeg_pipe_in_cmd.c_str(), "r");
    FILE *pipeout = popen(ffmpeg_pipe_out_cmd.c_str(), "w");

    const int W = std::stoi(width);
    const int H = std::stoi(height);
    char bitmap[W*H*3];
    unsigned char frame[H][W][3] = {0};

    cv::Mat mask;
    cv::Mat inverted_mask;
    int count;

    blue_edges_filter apply_blue_edgess(std::stoi(width), std::stoi(height));

    while(1)
    {
        count = fread( frame, 1, H*W*3, pipein );
        if ( count != H*W*3 ) break;

        memcpy( bitmap, frame, W*H*3 );

        cv::Mat matrix( H, W, CV_8UC3, bitmap );

        if (mask.empty() == 1){
          createMask(matrix, mask, inverted_mask, H, W);
        }

        // apply_blue_edgess(matrix, mask, inverted_mask);
        // cuda_apply_blue_edgess(matrix, mask, inverted_mask);
        blue_edges(matrix, mask, inverted_mask);
        unsigned char* processed_raw_frame = ( unsigned char* )( matrix.data );
        fwrite(processed_raw_frame, 1, H*W*3, pipeout);
    }

    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    #ifdef BENCH
      timestamp_t t1 = get_timestamp();
      double secs = (t1 - t0) / 1000000.0L;
      std::cout << "Processing time: " << secs << std::endl;
    #endif

    return 0;
}
