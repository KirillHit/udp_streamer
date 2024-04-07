#ifndef UDP_STREAMER_EXCEPTION_HPP_
#define UDP_STREAMER_EXCEPTION_HPP_

#include <stdexcept>
#include <string>


namespace udp_streamer
{

class UdpStreamerException : public std::runtime_error
{
public:
    UdpStreamerException(const char *file, int line, const std::string &arg)
    : std::runtime_error(arg)
    {
    msg_ = std::string(file) + ":" + std::to_string(line) + ": " + arg;
    }

    const char *what() const noexcept override
    {
    return msg_.c_str();
    }

private:
    std::string msg_;
};

}  // namespace udp_streamer

#endif  // UDP_STREAMER_EXCEPTION_HPP_