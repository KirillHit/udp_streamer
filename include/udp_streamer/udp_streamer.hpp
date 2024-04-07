#ifndef UDP_STREAMER_HPP_
#define UDP_STREAMER_HPP_

#include "udp_streamer_exception.hpp"
#include "simple_socket/simple_socket.hpp"
#include "opencv2/opencv.hpp"
#include <vector>

namespace udp_streamer
{


class StreamerBase
{
public:
    StreamerBase(const std::string& path_to_file);
    ~StreamerBase() = default;
    void load_config();
    void save_config();
    void set_size(const int width, const int height);
    void set_interval(const int interval);
    void set_pack_size(const int pack_size);
    void set_encode_quality(const int encode_quality);
protected:
    int frame_width_ = 1280;
    int frame_height_ = 720;
    int frame_interval_ = 1;
    int pack_size_ = 4096; //udp pack size (max 65535, note that osx limits < 8100 bytes)
    int encode_quality_ = 70;
    uint16_t port_ = 8000;
    std::string ip_address_ = "127.0.0.1";
private:
    std::string config_file;
};


class Transmitter : public StreamerBase, sockets::UDPClient
{
public:
    Transmitter(const std::string& path_to_file);
    ~Transmitter() = default;
    int send_img(const cv::Mat& img);
private:
    cv::Mat resize_img_;
    std::vector<uchar> buf_img_;
};


class Receiver : public StreamerBase, sockets::UDPServer
{
public:
    Receiver(const std::string& path_to_file);
    ~Receiver() = default;
    int receive_img(cv::Mat& output_img);
private:
    std::vector<uint8_t> buf;
};

}  // namespace udp_streamer

#endif  // UDP_STREAMER_HPP_