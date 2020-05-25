//#define _POSIX_C_SOURCE 200112L
#include "clientSocket.h"

#include <system_error>
#include <string>
#include <cstring>

//VER CUALES DE ESTOS INCLUDE HACEN FALTA
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "commonCourierSocket.h"

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

void ClientSocket::_obtain_addrinfo(const char* host, const char* service,
                                     struct addrinfo* hints,
                                     struct addrinfo** result){
  if (getaddrinfo(host, service, hints, result) != 0) {
    throw(std::system_error(0, std::generic_category(), "getaddrinfo error"));
  }
}

//////////////////////////////////PUBLIC/////////////////////////////////

//PASAR LOS THROW A LAS FUNCIONES AUXILIARES ASI DEVUELVEN VOID Y NO SE
//TIENEN IFS CHEQUEANDO SI SE EJECUTARON BIEN LAS FUNCIONES AUXILIARES
void ClientSocket::connect(const std::string& host, const std::string& service){
  bool is_connected = false;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = 0;
  if (host == "") {
    _obtain_addrinfo(NULL, service.data(), &hints, &result);
  } else {
    _obtain_addrinfo(host.data(), service.data(), &hints, &result);
  }
  is_connected = _process_info_to_connect(result, socket_fd);
  freeaddrinfo(result);
  if (!is_connected) {
    throw(std::system_error(errno, std::system_category(), "Connection error"));
  }
  c_socket.set_fd(socket_fd);
}

void ClientSocket::receive(void* buffer, size_t buffer_len) const{
  c_socket.receive(buffer, buffer_len);
}

void ClientSocket::send(const void* buffer, size_t buffer_len) const{
  c_socket.send(buffer, buffer_len);
}

ClientSocket::ClientSocket(): c_socket(){
}


ClientSocket::~ClientSocket(){
}
