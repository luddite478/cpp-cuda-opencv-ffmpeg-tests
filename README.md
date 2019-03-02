1. google-cloud -> marketplace NVIDIA GPU Cloud Image for Deep Learning and HPC

2. ```docker login nvcr.io``` -> username $oauthtoken -> password <NGC API KEY>

3. ``` docker build -t processing-module -f `pwd`/Dockerfile `pwd` ```

4. nvidia-smi

![alt text](https://github.com/luddite478/cpp-cuda-opencv-ffmpeg-tests/blob/master/nvidia-smi.png)

5. nano /etc/docker/daemon.json
```
    {
        "default-runtime": "nvidia", 
        "runtimes": {
            "nvidia": {
                "path": "nvidia-container-runtime",
                "runtimeArgs": []
            }
        }
    }
```
6. Create compiling container:
  ``` cd docker-compiler-img docker build -t cv_ff_compiler .```
