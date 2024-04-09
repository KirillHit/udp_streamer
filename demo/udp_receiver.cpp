#include "udp_streamer/udp_streamer.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

int main(int argc, char * argv[])
{
    std::string config_path;
    
    if (argc != 2) {
        std::cerr << "Err: Wrong command!\n  arg1: Path to the config file\n  arg2: Camera device path" << std::endl;
        exit(1);
    }

    config_path = argv[1];
    
    udp_streamer::Receiver receiver;
    receiver.load_config(config_path);
    receiver.save_config();

    cv::namedWindow("Receiver", cv::WINDOW_AUTOSIZE);

    cv::Mat output_img;

    while (true)
    {        
        receiver.receive_img(output_img);
        
        if (!output_img.empty()) {
            cv::imshow("Receiver", output_img);
        }

        if (cv::waitKey(1) == (int)'q') break;
    }
    
    return 0;
}