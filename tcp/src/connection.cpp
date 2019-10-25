#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>

#include <connection.hpp>
#include <tcp_exception.hpp>

constexpr int BUFFER_SIZE = 256;

ssize_t tcp::Connection::send(const std::string& message) {
  if (!ready) {
      throw ConnectionException("Connection not ready");
  }
  
  if (!open) {
      throw ConnectionException("Connection not open");
  }
    
  std::string prepped_message = "#|" + message + "|#";

  ssize_t bytes =
      ::send(fd, prepped_message.c_str(), prepped_message.length(), 0);

  if (bytes == -1) {
    throw tcp::SendException(message);
  }

  return bytes;
}

std::vector<std::string> tcp::Connection::receive(int flags) {
  if (!ready) {
      throw ConnectionException("Connection not ready");
  }
  
  if (!open) {
      throw ConnectionException("Connection not open");
  }
  
  char buffer[BUFFER_SIZE];

  while (true) {
    std::memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes = ::recv(fd, buffer, BUFFER_SIZE, flags);

    if (bytes == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK){
            break;
        } else {
            throw tcp::ReceiveException(errno);
        }
    } else if (bytes == 0) {
      break;
    } else {
      obuffer.append(buffer, bytes);
    }
  }

  size_t start_pos, end_pos;
  std::vector<std::string> messages;
  start_pos = obuffer.find("#|");
  end_pos = obuffer.find("|#");

  while (end_pos != std::string::npos) {
    if (start_pos != 0) {
      throw tcp::MalformedMessageException(obuffer);
    }
  
    messages.push_back(obuffer.substr(start_pos + 2, end_pos - 2));
    obuffer.erase(start_pos, end_pos + 2);
    
    start_pos = obuffer.find("#|");
    end_pos = obuffer.find("|#");
  }

  return messages;
}

void tcp::Connection::setFD(int fd) {
  this->fd = fd;
  ready = true;
}

bool tcp::Connection::closed() {
    return !ready || !open;
}

void tcp::Connection::close() {
  if (!open || ::close(fd) == -1) {
    throw tcp::CloseException();
  };
  
  open = false;
}

tcp::Connection::~Connection() {
  close();
}