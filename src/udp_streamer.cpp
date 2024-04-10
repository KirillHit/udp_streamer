#include "udp_streamer/udp_streamer.hpp"


namespace udp_streamer
{

Transmitter::Transmitter()
: UDPClient()
{}


void Transmitter::set_size(const int width, const int height)
{
    frame_width_ = cv::max(width, 320);
    frame_height_ = cv::max(frame_height_, 200);
}
void Transmitter::set_interval(const int interval)
{
    frame_interval_ = cv::max(interval, 0);
}
void Transmitter::set_pack_size(const int pack_size)
{
    pack_size_ = cv::max(pack_size, 1);
    pack_size_ = cv::min(pack_size, 65535);
}
void Transmitter::set_encode_quality(const int encode_quality)
{
    encode_quality_ = cv::max(encode_quality, 1);
    encode_quality_ = cv::min(encode_quality, 100);
}


int Transmitter::send_img(const cv::Mat& img)
{
    cv::resize(img, resize_img_, cv::Size(frame_width_, frame_height_), 0, 0, cv::INTER_LINEAR);

    std::vector<int> encode_params = {cv::IMWRITE_JPEG_QUALITY, encode_quality_};
    
    cv::imencode(".jpg", resize_img_, buf_img_, encode_params);

    int total_pack = (buf_img_.size() - 1) / pack_size_ + 1;
    char* proc_data =reinterpret_cast<char*>(buf_img_.data());

    send_mes(reinterpret_cast<char*>(&total_pack), sizeof(int));

    std::this_thread::sleep_for(std::chrono::microseconds(frame_interval_));

    for (int i = 0; i < total_pack; i++) {
        send_mes(proc_data, pack_size_);
        proc_data += pack_size_;
        std::this_thread::sleep_for(std::chrono::microseconds(frame_interval_));
    }
    
    send_mes(&EndId, 1);

    return 0;
}


Receiver::Receiver()
: UDPServer(), buf(sizeof(int))
{}


void Receiver::set_pack_size(const int pack_size)
{
    pack_size_ = cv::max(pack_size, 1);
    pack_size_ = cv::min(pack_size, 65535);
}


int Receiver::receive_img(cv::Mat& output_img)
{
    while (receive(buf.data(), buf.size()) != sizeof(int));

    int total_pack = *(reinterpret_cast<int*>(buf.data()));

    buf.resize(pack_size_ * total_pack);
    char* proc_data = buf.data();

    for (int i = 0; i < total_pack; i++) {
        if (receive(proc_data, pack_size_) != pack_size_) {
            // std::cerr << "Receive error, incorrect package size" << std::endl;
            return -1;
        }
        proc_data += pack_size_;
    }

    imdecode(buf, cv::IMREAD_COLOR, &output_img);

    return 0;
}


}  // namespace udp_streamer
