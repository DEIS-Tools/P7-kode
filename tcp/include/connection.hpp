#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace tcp {

class SendException : public std::exception {
    std::string message;

  public:
    SendException(const std::string &in_message) : message(in_message) {}

    const char *what() const noexcept
    {
        return std::string("Could not send message: " + message).c_str();
    }
};

class ReceiveException : public std::exception {
    int err;
    std::string msg;

  public:
    ReceiveException(int err)
        : err(err), msg("Could not read receive buffer: " + std::to_string(err))
    {
    }

    const char *what() const noexcept { return msg.c_str(); }
};

struct MalformedMessageException : public std::exception {
    std::string message;

  public:
    MalformedMessageException(const std::string &in_message)
        : message(std::string("Missing start sequence: " + in_message))
    {
    }

    const char *what() const noexcept { return message.c_str(); }
};

class Connection : public std::enable_shared_from_this<Connection> {
  public:
    Connection(int fd) : fd(fd) { ready = true; }
    Connection() {}
    virtual ~Connection();
    std::vector<std::string> receive(int flags = 0);
    ssize_t send(const std::string &message);
    void close();

    bool closed();

  protected:
    void set_fd(int fd);
    void read_buffer();
    std::vector<std::string> parse_messages();

  private:
    int fd;
    std::string obuffer;
    bool open = true;
    bool ready;

    friend class Server;
};

} // namespace tcp

#endif // TCP_CONNECTION_HPP
