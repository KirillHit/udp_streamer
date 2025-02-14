#ifndef UDP_STREAMER_HPP_
#define UDP_STREAMER_HPP_

#include "opencv2/opencv.hpp"
#include "simple_socket/simple_socket.hpp"
#include "udp_streamer_exception.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

namespace udp_streamer
{

const uint8_t START_ID = 0x5A;
#pragma pack(push, 1)
struct StartMsg
{
    uint8_t start_id = START_ID;
    uint16_t pack_size;
    uint16_t total_pack;
};
#pragma pack(pop)
const size_t MSG_SIZE = sizeof(StartMsg);

class Transmitter : public sockets::UDPClient
{
  public:
    Transmitter();
    ~Transmitter() = default;
    int send_img(const cv::Mat &img);
    void set_interval(const int interval);
    void set_pack_size(const int pack_size);
    void set_encode_quality(const int encode_quality);

  private:
    // udp pack_size (max 65535, note that osx limits < 8100 bytes)
    int pack_size_ = 4096;
    int frame_interval_ = 0;
    int encode_quality_ = 70;
    std::vector<unsigned char> buf_img_;
    StartMsg tx_msg_;
};

class Receiver : public sockets::UDPServer
{
  public:
    Receiver();
    ~Receiver() = default;
    int receive_img(cv::Mat &output_img);

  private:
    std::vector<char> buf;
    StartMsg rx_msg_;
};

} // namespace udp_streamer

#endif // UDP_STREAMER_HPP_
