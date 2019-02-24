#!/bin/sh
src="$1"
bin="$2"

cd /usr/stream && g++ -std=c++11 -o $bin $src `pkg-config opencv --cflags --libs`
