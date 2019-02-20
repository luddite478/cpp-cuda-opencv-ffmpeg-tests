1.google-cloud -> marketplace NVIDIA GPU Cloud Image for Deep Learning and HPC

2. docker build -t cuda-container-2 -f \`pwd\`/Dockerfile \`pwd\`

3. nvidia-smi

![alt text](https://github.com/luddite478/cpp-cuda-opencv-ffmpeg-tests/blob/master/nvidia-smi.png)

4. docker run --runtime=nvidia -ti -v /<host_folder>:/<container_folder> cuda-container sh

5. cd <container_folder> && g++ -o app app.cpp \`pkg-config opencv --cflags --libs\`
