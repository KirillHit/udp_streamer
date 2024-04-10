# udp_streamer
A library for transmitting video from a camera over UDP with minimal delays. The open-source library [OpenCV](https://opencv.org/) is used to capture and encode images. This project is inspired by [udp-image-streaming](https://github.com/chenxiaoqino/udp-image-streaming/tree/master).

### Demo
The "demo" folder contains examples of receiver and transmitter code. Build and run the demo:

``` bash
mkdir build
cd build
cmake -DDemo:=ON ..
cmake --build .
cd Demo
# First terminal
.\udp_transmitter
# second terminal
.\udp_receiver
```

