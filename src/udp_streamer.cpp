#include "udp_streamer/udp_streamer.hpp"
#include <chrono>
#include <thread>


namespace udp_streamer
{


StreamerBase::StreamerBase(const std::string& path_to_file)
: config_file{path_to_file}
{
    load_config();
}


void StreamerBase::load_config()
{
    cv::FileStorage fs;
    
    try {
        fs.open(config_file, cv::FileStorage::READ);
    }
    catch(const cv::Exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }
    
    if (!fs.isOpened()) {
        std::cerr << "Failed to open: " << config_file << std::endl;
        return;
    }
    
    cv::FileNode streamer_config = fs["udp_streamer"];

    if (!streamer_config.isMap()) {
        std::cerr << "Empty config threshold: " << config_file << std::endl;
        return;
    }

    streamer_config["frame_height"] >> frame_height_;
    streamer_config["frame_width"] >> frame_width_;
    streamer_config["frame_interval"] >> frame_interval_;
    streamer_config["pack_size"] >> pack_size_;
    streamer_config["encode_quality"] >> encode_quality_;
    streamer_config["port"] >> port_;
    streamer_config["ip_address"] >> ip_address_;

    frame_width_ = cv::max(frame_width_, 320);
    frame_height_ = cv::max(frame_height_, 200);
    frame_interval_ = cv::max(frame_interval_, 0);
    pack_size_ = cv::max(pack_size_, 1);
    pack_size_ = cv::min(pack_size_, 65535);
    encode_quality_ = cv::max(encode_quality_, 1);
    encode_quality_ = cv::min(encode_quality_, 100);

    fs.release();
}


void StreamerBase::save_config()
{
    cv::FileStorage fs;
    
    try {
        fs.open(config_file, cv::FileStorage::WRITE);
    }
    catch(const cv::Exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }

    fs << "udp_streamer";
    fs << "{";
    fs << "port" << port_;
    fs << "ip_address" << ip_address_;
    fs << "pack_size" << pack_size_;
    fs.writeComment("only transmitter", 0);
    fs << "frame_width" << frame_width_;
    fs << "frame_height" << frame_height_;
    fs << "frame_interval" << frame_interval_;
    fs.writeComment("interval between sending packets, us", 1);
    fs << "encode_quality" << encode_quality_;
    fs << "}";

    fs.release();
}


void StreamerBase::set_size(const int width, const int height)
{
    frame_width_ = cv::max(width, 320);
    frame_height_ = cv::max(frame_height_, 200);
}
void StreamerBase::set_interval(const int interval)
{
    frame_interval_ = cv::max(interval, 0);
}
void StreamerBase::set_pack_size(const int pack_size)
{
    pack_size_ = cv::max(pack_size, 1);
    pack_size_ = cv::min(pack_size, 65535);
}
void StreamerBase::set_encode_quality(const int encode_quality)
{
    encode_quality_ = cv::max(encode_quality, 1);
    encode_quality_ = cv::min(encode_quality, 100);
}


Transmitter::Transmitter(const std::string& path_to_file)
: StreamerBase(path_to_file), UDPClient(port_, ip_address_)
{}


int Transmitter::send_img(const cv::Mat& img)
{
    cv::resize(img, resize_img_, cv::Size(frame_width_, frame_height_), 0, 0, cv::INTER_LINEAR);

    std::vector<int> encode_params = {cv::IMWRITE_JPEG_QUALITY, encode_quality_};
    
    cv::imencode(".jpg", resize_img_, buf_img_, encode_params);

    int total_pack = (buf_img_.size() - 1) / pack_size_ + 1;
    uchar* proc_data = buf_img_.data();

    send_mes(reinterpret_cast<uint8_t*>(&total_pack), sizeof(int));

    for (int i = 0; i < total_pack; i++) {
        send_mes(proc_data, pack_size_);
        proc_data += pack_size_;
        std::this_thread::sleep_for(std::chrono::microseconds(frame_interval_));
    }

    uint8_t end_id = 0xED;
    send_mes(&end_id, 1);

    return 0;
}


Receiver::Receiver(const std::string& path_to_file)
: StreamerBase(path_to_file), UDPServer(port_, ip_address_), buf(sizeof(int))
{
    socket_bind();
}


int Receiver::receive_img(cv::Mat& output_img)
{
    while (receive(buf.data(), buf.size()) != sizeof(int));

    int total_pack = *((int*) buf.data());

    buf.resize(pack_size_ * total_pack);
    uchar* proc_data = buf.data();

    for (int i = 0; i < total_pack; i++) {
        if (receive(proc_data, pack_size_) != pack_size_) {
            std::cerr << "Receive error, incorrect package size" << std::endl;
            return -1;
        }
        proc_data += pack_size_;
    }

    imdecode(buf, cv::IMREAD_COLOR, &output_img);

    return 0;
}


}  // namespace udp_streamer
