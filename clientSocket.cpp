#include "clientSocket.h"

#include <system_error>
#include <string>
#include <cstring>
#include <utility>
#include <netdb.h>
#include <unistd.h>

#include "commonCourierSocket.h"

//Tries to establish a connection using the info received, returns true
//if it acomplishes it, otherwise returns false
bool ClientSocket::_could_connect(struct addrinfo* info, int& socket_fd){
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


//Tries to get a socket with the information provided by info
//and tries to establish a connection
bool ClientSocket::_process_info_to_connect(struct addrinfo* info,
                                            int& socket_fd){
  bool is_connected = false;
  while ((info != NULL) && (!is_connected)) {
    is_connected = _could_connect(info, socket_fd);
    info = info->ai_next;
  }
  return is_connected;
}

//Initializes the hints with the desired prefferences
void ClientSocket::_set_hints(struct addrinfo* hints){
  std::memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
}

//Stores in result the sockets available for connection with the
//prefferences indicated in hints
void ClientSocket::_obtain_addrinfo(const char* host, const char* service,
                                     struct addrinfo* hints,
                                     struct addrinfo** result){
  if (getaddrinfo(host, service, hints, result) != 0) {
    throw(std::system_error(0, std::generic_category(), "getaddrinfo error"));
  }
}

//Establishes a connection with a socket that belongs to the received service
//and host
void ClientSocket::_connect(const char* service, const char* host){
  bool is_connected = false;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = 0;
  _obtain_addrinfo(host, service, &hints, &result);
  is_connected = _process_info_to_connect(result, socket_fd);
  freeaddrinfo(result);
  if (!is_connected) {
    throw(std::system_error(errno, std::system_category(),
                            "Connection error"));
  }
  CourierSocket aux_c_socket(socket_fd);
  c_socket = std::move(aux_c_socket);
}


//////////////////////////////////INHERITED/////////////////////////////////
//Establishes connection between the client server and a socket from the
//adress received in the constructor
void ClientSocket::allow_communication(const char* service, const char* host){
  _connect(service, host);
}

//////////////////////////////////PUBLIC/////////////////////////////////


void ClientSocket::receive(void* buffer, size_t buffer_len) const{
  c_socket.receive(buffer, buffer_len);
}

void ClientSocket::send(const void* buffer, size_t buffer_len) const{
  c_socket.send(buffer, buffer_len);
}

ClientSocket::ClientSocket(const std::string& service,
                           const std::string& host):
                           Communicator(service.data(), host.data()){
}


ClientSocket::~ClientSocket(){
}
