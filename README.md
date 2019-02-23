1. google-cloud -> marketplace NVIDIA GPU Cloud Image for Deep Learning and HPC

2. docker login nvcr.io -> username $oauthtoken -> password <NGC API KEY>

3. docker build -t cuda-container-2 -f \`pwd\`/Dockerfile \`pwd\`

4. nvidia-smi

![alt text](https://github.com/luddite478/cpp-cuda-opencv-ffmpeg-tests/blob/master/nvidia-smi.png)

5. docker run --runtime=nvidia -ti -v /<host_folder>:/<container_folder> cuda-container sh

6. cd <container_folder> && g++ -o app app.cpp \`pkg-config opencv --cflags --libs\`
