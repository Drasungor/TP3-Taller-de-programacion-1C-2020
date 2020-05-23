#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "socket.h"


#define SUCCESS 0
#define ERROR -1
#define CLOSED_SOCKET -2
#define INVALID_ACTION -3


//ESTO ES DEL CLIENT
//Tries a getspecific socket and establish a connection, returns true
//if it acomplishes, otherwise returns false
static bool _could_connect(struct addrinfo* info, int *socket_fd){
  int connect_value = 0;
  *socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (*socket_fd != -1) {
    connect_value = connect(*socket_fd, info->ai_addr, info->ai_addrlen);
    if (connect_value == -1) {
      close(*socket_fd);
    } else{
      return true;
    }
  }
  return false;
}

//ESTO ES DEL CLIENT
//Tries to get a socket with the information provided by info
//and tries to establish a connection
static bool _process_info_to_connect(struct addrinfo* info, int *socket_fd){
  bool is_connected = false;
  while ((info != NULL) && (!is_connected)) {
    is_connected = _could_connect(info, socket_fd);
    info = info->ai_next;
  }
  return is_connected;
}

//ESTO VA EN EL DESTRUCTOR DEL SERVER
//This function can't be reduced to 15 lines due to error
//checking
//Tries get a socket and bind, returns true
//if it acomplishes, otherwise returns false
static bool _could_bind(struct addrinfo* info, int *socket_fd){
  int val = 1;
  int set_value = 0;
  int bind_value = 0;
  *socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (*socket_fd != -1) {
    set_value = setsockopt(*socket_fd,
                           SOL_SOCKET,
                           SO_REUSEADDR,
                           &val,
                           sizeof(int));
    if (set_value == -1) {
      close(*socket_fd);
    } else {
      bind_value = bind(*socket_fd, info->ai_addr, info->ai_addrlen);
      if (bind_value == -1) {
        close(*socket_fd);
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
static bool _process_info_to_bind(struct addrinfo* info, int *socket_fd){
  bool is_bound = false;
  while ((info != NULL) && (!is_bound)) {
    is_bound = _could_bind(info, socket_fd);
    info = info->ai_next;
  }
  return is_bound;
}


static void _set_hints(struct addrinfo *hints){
  memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
}


void socket_init(socket_t *sckt){
  sckt->fd = -1;
  sckt->is_client = false;
  sckt->is_server = false;
  sckt->client_fd = 0;
}


//ESTO ES PARTE DEL SOCKET SERVER
//This function can't be reduced to 15 lines bcause of error
//checking and initializacion
int socket_bind_and_listen(socket_t *sckt, const char *service){
  if (sckt->is_client) {
    return false;
  }

  int info_result = 0;
  bool is_bound = false;
  int listen_value = 0;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = AI_PASSIVE;

  info_result = getaddrinfo(NULL, service, &hints, &result);
  if (info_result != 0) {
    return info_result;
  }
  is_bound = _process_info_to_bind(result, &socket_fd);
  freeaddrinfo(result);
  if (!is_bound) {
    return ERROR;
  }
  sckt->fd = socket_fd;
  listen_value = listen(sckt->fd, 1);
  if (listen_value != 0) {
    return ERROR;
  }
  sckt->is_server = true;
  return SUCCESS;
}

//ESTO ES PARTE DEL SOCKET SERVER
int socket_accept(socket_t *sckt){
  if (!sckt->is_server) {
    return INVALID_ACTION;
  }
  sckt->client_fd = accept(sckt->fd, NULL, NULL);
  if (sckt->client_fd == -1) {
    return ERROR;
  }
  return SUCCESS;
}

//ESTO ES PARTE DEL SOCKET CLIENTE
//This function can't be reduced to 15 lines bcause of error
//checking and initializacion
int socket_connect(socket_t *sckt, const char *host, const char *service){
  if (sckt->is_server) {
    return false;
  }
  int info_result = 0;
  bool is_connected = false;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  _set_hints(&hints);
  hints.ai_flags = 0;

  info_result = getaddrinfo(host, service, &hints, &result);
  if (info_result != 0) {
    return info_result;
  }
  is_connected = _process_info_to_connect(result, &socket_fd);
  freeaddrinfo(result);
  if (!is_connected) {
    return ERROR;
  }
  sckt->fd = socket_fd;
  sckt->is_client = true;
  return SUCCESS;
}


///////////////////////////////PUBLIC//////////////////////////

void CommunicationSocket::receive(void* buffer, int buffer_len) const{
  int total_bytes_received = 0;
  int current_bytes_received = 0;
  char* current_address = (char*)buffer;
  while (total_bytes_received < len) {
    current_bytes_received = recv(socket_fd, current_address,
                                  len - total_bytes_received,
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

void CommunicationSocket::send(const void* buffer, int buffer_len) const{
  size_t total_bytes_sent = 0;
  size_t current_bytes_sent = 0;
  const char* current_address = (const char*)buffer;
  while (total_bytes_sent < len) {
    current_bytes_sent = send(sckt_fd, current_address, len - total_bytes_sent,
                              MSG_NOSIGNAL);
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

void CommunicationSocket::set_fs(int fd){
  sckt_fd = fd;
}


CommunicationSocket::CommunicationSocket(){
  sckt_fd = -1;
}

/*
CommunicationSocket::CommunicationSocket(CommunicationSocket&& other) noexcept{
  //IMPLEMENTAR
}
*/

virtual CommunicationSocket::~CommunicationSocket(){
  shutdown(sckt_fd, SHUT_RDWR);
  close(sckt_fd);
}
