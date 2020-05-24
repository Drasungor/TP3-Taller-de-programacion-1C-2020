//#define _POSIX_C_SOURCE 200112L
#include "clientSocket.h"

#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


//ESTO ES DEL CLIENT
//Tries a getspecific socket and establish a connection, returns true
//if it acomplishes, otherwise returns false
static bool _could_connect(struct addrinfo* info, int& socket_fd){
  int connect_value = 0;
  socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (socket_fd != -1) {
    connect_value = connect(socket_fd, info->ai_addr, info->ai_addrlen);
    if (connect_value == -1) {
      close(socket_fd);
    } else{
      return true;
    }
  }
  return false;
}

//ESTO ES DEL CLIENT
//Tries to get a socket with the information provided by info
//and tries to establish a connection
bool ClientSocket::_process_info_to_connect(struct addrinfo* info, int& socket_fd){
  bool is_connected = false;
  while ((info != NULL) && (!is_connected)) {
    is_connected = _could_connect(info, socket_fd);
    info = info->ai_next;
  }
  return is_connected;
}


void ClientSocket::_set_hints(struct addrinfo* hints){
  std::memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
}



//////////////////////////////////PUBLIC/////////////////////////////////

//PASAR LOS THROW A LAS FUNCIONES AUXILIARES ASI DEVUELVEN VOID Y NO SE
//TIENEN IFS CHEQUEANDO SI SE EJECUTARON BIEN LAS FUNCIONES AUXILIARES
void ClientSocket::connect(const std::string& host, const std::string& service){
  bool is_connected = false;
  int socket_fd = 0;
  char* host_ptr = NULL;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = 0;
  if (host != "") {
    host_ptr = host.data();
  }
  if (getaddrinfo(host_ptr, service.data(), &hints, &result) != 0) {
    throw(std::ios_base::failure);
  }
  is_connected = _process_info_to_connect(result, socket_fd);
  freeaddrinfo(result);
  if (!is_connected) {
    throw(std::ios_base::failure);
  }
  set_fd(socket_fd);
}


ClientSocket::ClientSocket(): CommunicationSocket(){
}


ClientSocket::~ClientSocket(){
}
