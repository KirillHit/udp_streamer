#include "udp_streamer/udp_streamer.hpp"

namespace udp_streamer
{

Transmitter::Transmitter() : UDPClient()
{
    tx_msg_.pack_size = pack_size_;
}

void Transmitter::set_interval(const int interval)
{
    frame_interval_ = cv::max(interval, 0);
}

void Transmitter::set_pack_size(const int pack_size)
{
    pack_size_ = cv::max(pack_size, 1);
    pack_size_ = cv::min(pack_size, 65535);
    tx_msg_.pack_size = pack_size_;
}

void Transmitter::set_encode_quality(const int encode_quality)
{
    encode_quality_ = cv::max(encode_quality, 1);
    encode_quality_ = cv::min(encode_quality, 100);
}

int Transmitter::send_img(const cv::Mat &img)
{
    std::vector<int> encode_params = {cv::IMWRITE_JPEG_QUALITY,
                                      encode_quality_};

    cv::imencode(".jpg", img, buf_img_, encode_params);

    uint16_t total_pack = (buf_img_.size() - 1) / pack_size_ + 1;
    tx_msg_.total_pack = total_pack;

    send_mes(reinterpret_cast<char *>(&tx_msg_), MSG_SIZE);
    std::this_thread::sleep_for(std::chrono::microseconds(frame_interval_));

    char *proc_data = reinterpret_cast<char *>(buf_img_.data());

    for (int i = 0; i < total_pack; i++)
    {
        send_mes(proc_data, pack_size_);
        proc_data += pack_size_;
        std::this_thread::sleep_for(std::chrono::microseconds(frame_interval_));
    }

    return 0;
}

Receiver::Receiver() : UDPServer()
{
    set_timeout(1);
}

int Receiver::receive_img(cv::Mat &output_img)
{
    if (receive(reinterpret_cast<char *>(&rx_msg_), MSG_SIZE) <= 0)
    {
        return -1;
    }

    if (rx_msg_.start_id != START_ID)
    {
        return -1;
    }

    buf.resize(rx_msg_.pack_size * rx_msg_.total_pack);
    char *proc_data = buf.data();

    for (int i = 0; i < rx_msg_.total_pack; i++)
    {
        if (receive(proc_data, rx_msg_.pack_size) != rx_msg_.pack_size)
        {
            return -1;
        }
        proc_data += rx_msg_.pack_size;
    }

    imdecode(buf, cv::IMREAD_COLOR, &output_img);

    return 0;
}

} // namespace udp_streamer
