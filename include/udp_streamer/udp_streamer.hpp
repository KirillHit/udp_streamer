#ifndef UDP_STREAMER_HPP_
#define UDP_STREAMER_HPP_

#include "udp_streamer_exception.hpp"
#include "simple_socket/simple_socket.hpp"
#include "opencv2/opencv.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <cstdint>
#include <iostream>


namespace udp_streamer
{


class StreamerBase
{
public:
    StreamerBase() = default;
    ~StreamerBase() = default;
    void load_config(const std::string& path_to_file = "");
    void save_config(const std::string& path_to_file = "");
    void set_size(const int width, const int height);
    void set_interval(const int interval);
    void set_pack_size(const int pack_size);
    void set_encode_quality(const int encode_quality);
protected:
    int frame_width_ = 720;
    int frame_height_ = 540;
    int frame_interval_ = 50;
    int pack_size_ = 4096; //udp pack size (max 65535, note that osx limits < 8100 bytes)
    int encode_quality_ = 70;
    uint16_t port_ = 8000;
    std::string ip_address_ = "127.0.0.1";
    const char end_id = 0xED;
private:
    std::string config_file;
};


class Transmitter : public StreamerBase, sockets::UDPClient
{
public:
    Transmitter();
    ~Transmitter() = default;
    int send_img(const cv::Mat& img);
private:
    cv::Mat resize_img_;
    std::vector<unsigned char> buf_img_;
};


class Receiver : public StreamerBase, sockets::UDPServer
{
public:
    Receiver();
    ~Receiver() = default;
    int receive_img(cv::Mat& output_img);
private:
    std::vector<char> buf;
};

}  // namespace udp_streamer

#endif  // UDP_STREAMER_HPP_