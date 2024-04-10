#include "udp_streamer/udp_streamer.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

int main()
{
    udp_streamer::Transmitter transmitter;
    transmitter.set_socket("127.0.0.1", 8000);
    transmitter.set_pack_size(4096);
    transmitter.set_size(720, 480);
    transmitter.set_interval(50);
    transmitter.set_encode_quality(70);

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Err: Failed to open camera" << std::endl;
        exit(1);
    }
    cv::namedWindow("Transmitter", cv::WINDOW_AUTOSIZE);
    cv::Mat original_img;

    while (true)
    {
        if (!cap.read(original_img)) { 
            std::cerr << "Err: Read camera error" << std::endl;
            break;
        }

        if (original_img.empty()) { 
            std::cerr << "Err: No data" << std::endl;
            break;
        }

        cv::imshow("Transmitter", original_img);

        transmitter.send_img(original_img);

        if (cv::waitKey(1) == (int)'q') break;
    }
    
    return 0;
}