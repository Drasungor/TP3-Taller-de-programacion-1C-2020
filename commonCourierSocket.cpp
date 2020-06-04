#include "commonCourierSocket.h"

#include <cstddef>
#include <system_error>
#include <netdb.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR -1
#define CLOSED_SOCKET -2
#define INVALID_ACTION -3



///////////////////////////////PUBLIC//////////////////////////

void CourierSocket::receive(void* buffer, size_t buffer_len) const{
  size_t total_bytes_received = 0;
  size_t current_bytes_received = 0;
  char* current_address = (char*)buffer;
  while (total_bytes_received < buffer_len) {
    current_bytes_received = recv(socket_fd, current_address,
                                  buffer_len - total_bytes_received,
                                  MSG_NOSIGNAL);
    if (current_bytes_received <= 0) {
      throw(std::system_error(errno, std::system_category(), "Receive error"));
    }
    current_address += current_bytes_received;
    total_bytes_received += current_bytes_received;
  }
}


void CourierSocket::send(const void* buffer, size_t buffer_len) const{
  size_t total_bytes_sent = 0;
  size_t current_bytes_sent = 0;
  const char* current_address = (const char*)buffer;
  while (total_bytes_sent < buffer_len) {
    current_bytes_sent = ::send(socket_fd, current_address,
                              buffer_len - total_bytes_sent, MSG_NOSIGNAL);
    if (current_bytes_sent <= 0) {
      throw(std::system_error(errno, std::system_category(), "Send error"));
    }
    current_address += current_bytes_sent;
    total_bytes_sent += current_bytes_sent;
  }
}


CourierSocket::CourierSocket(int fd/* = -1*/) noexcept{
  socket_fd = fd;
}

CourierSocket::CourierSocket(CourierSocket&& other) noexcept{
  this->socket_fd = other.socket_fd;
  other.socket_fd = -1;
}

CourierSocket& CourierSocket::operator=(CourierSocket&& other) noexcept{
  this->socket_fd = other.socket_fd;
  other.socket_fd = -1;
  return *this;
}

CourierSocket::~CourierSocket(){
  if (socket_fd != -1) {
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
  }
}
