#ifndef BLUE_EDGES_H
#define BLUE_EDGES_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"
#include "blue_edges.h"

void blue_edges(cv::Mat& matrix, cv::Mat& mask, cv::Mat& inverted_mask);

#endif
