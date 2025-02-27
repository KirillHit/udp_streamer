#include "opencv2/opencv.hpp"
#include "udp_streamer/udp_streamer.hpp"
#include <iostream>

int main()
{
    udp_streamer::Receiver receiver;
    receiver.set_socket("", 8000);
    receiver.socket_bind();

    cv::namedWindow("Receiver", cv::WINDOW_AUTOSIZE);

    cv::Mat output_img;

    while (true)
    {
        receiver.receive_img(output_img);

        if (!output_img.empty())
        {
            cv::imshow("Receiver", output_img);
        }

        if (cv::waitKey(1) == (int)'q')
        {
            break;
        }
    }

    return 0;
}