//#define _POSIX_C_SOURCE 200112L

#include "commonCommunicationSocket.h"

#include <cstddef>

//VER SI HACEN FALTA ESTOS INCLUDES
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR -1
#define CLOSED_SOCKET -2
#define INVALID_ACTION -3



///////////////////////////////PUBLIC//////////////////////////

void CommunicationSocket::receive(void* buffer, size_t buffer_len) const{
  size_t total_bytes_received = 0;
  size_t current_bytes_received = 0;
  char* current_address = (char*)buffer;
  while (total_bytes_received < buffer_len) {
    current_bytes_received = recv(socket_fd, current_address,
                                  buffer_len - total_bytes_received,
                                  MSG_NOSIGNAL);
    if (current_bytes_received == 0) {
      return /*CLOSED_SOCKET*//*TIRAR EXCEPTION DE SOCKET CERRADO*/;
    }
    //VER SI SE CAMBIA POR ELSE IF PARA QUEDAR EN 15 LINEAS
    if (current_bytes_received < 0) {
      return /*ERROR*//*TIRAR EXCEPTION DE ERROR DE COMUNICACION*/;
    }
    current_address += current_bytes_received;
    total_bytes_received += current_bytes_received;
  }
  //return SUCCESS;
}

void CommunicationSocket::send(const void* buffer, size_t buffer_len) const{
  size_t total_bytes_sent = 0;
  size_t current_bytes_sent = 0;
  const char* current_address = (const char*)buffer;
  while (total_bytes_sent < buffer_len) {
    current_bytes_sent = ::send(socket_fd, current_address,
                              buffer_len - total_bytes_sent, MSG_NOSIGNAL);
    if (current_bytes_sent == 0) {
      return /*CLOSED_SOCKET*//*TIRAR EXCEPTION DE SOCKET CERRADO*/;
    }
    if (current_bytes_sent < 0) {
      return /*ERROR*//*TIRAR EXCEPTION DE ERROR DE COMUNICACION*/;
    }
    current_address += current_bytes_sent;
    total_bytes_sent += current_bytes_sent;
  }
  //return SUCCESS;
}

void CommunicationSocket::set_fd(int fd){
  socket_fd = fd;
}


CommunicationSocket::CommunicationSocket() noexcept{
  socket_fd = -1;
}

/*
CommunicationSocket::CommunicationSocket(CommunicationSocket&& other) noexcept{
  //IMPLEMENTAR
}
*/

CommunicationSocket::~CommunicationSocket(){
  shutdown(socket_fd, SHUT_RDWR);
  close(socket_fd);
}
