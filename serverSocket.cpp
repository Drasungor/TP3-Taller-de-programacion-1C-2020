#include "serverSocket.h"

#include <cstring>
#include <system_error>
#include <string>
//VER CUALES DE ESTOS DEFINES SE PUEDEN SACAR
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "serverPeerSocket.h"

#define MAX_SIZE_WAITING_CLIENTS_QUEUE 10

//This function can't be reduced to 15 lines due to error
//checking
//Tries get a socket and bind, returns true
//if it acomplishes, otherwise returns false
bool ServerSocket::_could_bind(struct addrinfo* info, int& socket_fd){
  int val = 1;
  int set_value = 0;
  int bind_value = 0;
  socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (socket_fd != -1) {
    set_value = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &val,
                           sizeof(int));
    if (set_value == -1) {
      close(socket_fd);
    } else {
      bind_value = bind(socket_fd, info->ai_addr, info->ai_addrlen);
      if (bind_value == -1) {
        close(socket_fd);
      } else{
        return true;
      }
    }
  }
  return false;
}


//Tries to get a socket with the information provided by info
//and tries to bind this socket to the address and listen to
//connections
bool ServerSocket::_process_info_to_bind(struct addrinfo* info, int& socket_fd){
  bool is_bound = false;
  while ((info != NULL) && (!is_bound)) {
    is_bound = _could_bind(info, socket_fd);
    info = info->ai_next;
  }
  return is_bound;
}


void ServerSocket::_set_hints(struct addrinfo* hints){
  std::memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
}


//PASAR LOS THROW A LAS FUNCIONES AUXILIARES ASI DEVUELVEN VOID Y NO SE
//TIENEN IFS CHEQUEANDO SI SE EJECUTARON BIEN LAS FUNCIONES AUXILIARES
//void ServerSocket::bind_and_listen(const std::string& service){
void ServerSocket::_bind_and_listen(const char* service, const char* host){
  bool is_bound = false;
  int listen_value = 0;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(host /*=NULL*/, service, &hints, &result) != 0) {
    throw(std::system_error(0, std::generic_category(), "getaddrinfo error"));
  }
  is_bound = _process_info_to_bind(result, socket_fd);
  freeaddrinfo(result);
  if (!is_bound) {
    throw(std::system_error(errno, std::system_category(), "Binding error"));
  }
  this->socket_fd = socket_fd;
  listen_value = listen(this->socket_fd, MAX_SIZE_WAITING_CLIENTS_QUEUE);
  if (listen_value != 0) {
    throw(std::system_error(errno, std::system_category(), "Listen error"));
  }
}

//////////////////////////INHERITED//////////////////////////
void ServerSocket::allow_communication(const char* service, const char* host){
  _bind_and_listen(service, host);
}

///////////////////////////////PUBLIC//////////////////////////


PeerSocket ServerSocket::accept(){
  int aux = ::accept(this->socket_fd, NULL, NULL);
  if (aux == -1) {
    throw(std::system_error(errno, std::system_category(), "Accept error"));
  }
  PeerSocket peer_socket(aux);
  return peer_socket;
}

void ServerSocket::disconnect(){
  if (socket_fd != -1) {
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
  }
  socket_fd = -1;
}

/*
ServerSocket::ServerSocket(){
  socket_fd = -1;
}
*/
//Sets host to null in the Communicator's constructor due to default value
ServerSocket::ServerSocket(const std::string& service):
                           Communicator(service.data()){
  socket_fd = -1;
}


ServerSocket::~ServerSocket(){
  /*
  if (socket_fd != -1) {
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
  }
  */
  disconnect();
}














//asdasdas
