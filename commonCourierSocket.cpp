#include "commonCourierSocket.h"

#include <cstddef>
#include <system_error>

//VER SI HACEN FALTA ESTOS INCLUDES
//#include <sys/types.h>
//#include <sys/socket.h>
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

void CourierSocket::set_fd(int fd){
  socket_fd = fd;
}

//VER COMO CAMBIAR ESTO POR OTRA COSA QUE NO SEA UN GET
int CourierSocket::get_fd() const{
  return socket_fd;
}

CourierSocket::CourierSocket() noexcept{
  socket_fd = -1;
}

/*
CommunicationSocket::CommunicationSocket(CommunicationSocket&& other) noexcept{
  //IMPLEMENTAR
}
*/

CourierSocket::~CourierSocket(){
  if (socket_fd != -1) {
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
  }
}
