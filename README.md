1.google-cloud -> marketplace
NVIDIA GPU Cloud Image for Deep Learning and HPC

![alt text](https://github.com/luddite478/cpp-cuda-opencv-ffmpeg-tests/edit/master/nvidia-smi.png)

2. docker build -t cuda-container Dockerfile

3. docker run --runtime=nvidia -ti -v /<host_folder>:/<container_folder> cuda-container sh
