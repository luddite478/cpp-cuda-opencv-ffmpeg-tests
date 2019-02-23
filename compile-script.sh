#!/bin/bash

dst_bin=$1
src_folder="../cpp-cuda-opencv-ffmpeg-tests"
src="app.cpp"

gpp_cmd="g++ -std=c++11 -o $dst_bin $src \`pkg-config opencv --cflags --libs\`"

docker run --rm --runtime=nvidia -v `pwd`:/usr/stream processing-module bash -c "$gpp_cmd && cp $dst_bin /usr/stream/$dst_bin"
