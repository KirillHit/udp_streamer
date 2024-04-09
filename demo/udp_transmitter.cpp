#include "udp_streamer/udp_streamer.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

int main(int argc, char * argv[])
{
    std::string config_path;
    int video_capture_path;
    if (argc != 3) {
        std::cerr << "Err: Wrong command!\n  arg1: Path to the config file\n  arg2: Camera number" << std::endl;
        exit(1);
    }
    config_path = argv[1];
    video_capture_path = atoi(argv[2]);
    
    udp_streamer::Transmitter transmitter;
    transmitter.load_config(config_path);
    transmitter.save_config();

    cv::VideoCapture cap(video_capture_path);
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