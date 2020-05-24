#include "serverPeerSocket.h"

/*
//BORRAR INCLUDES, ESTO ES PARA EL PROXY
#include <iostream>
#include <string>
*/
///////////////////////////////PUBLIC//////////////////////////

/*
//DESPUES DE BORRAR EL BEGINNING AGREGARLE CONST A LA FUNCION
void PeerSocket::receive(void* buffer, int buffer_len){
  char* buff_string = (char*)buffer;
  for (int i = 0; i < buffer_len; i++) {
    buff_string[i] = message[i + beginning];
  }
  beginning += buffer_len;
}

//REIMPLEMENTAR, ES SOLO PARA PROBAR LA LOGICA DEL PROGRAMA
void PeerSocket::send(const void* buffer, int buffer_len) const{

  //std::cout << "MENSAJE ENVIADO POR EL SOCKET" << std::endl;

  char* message = (char*)buffer;
  for (int i = 0; i < buffer_len; i++) {
    std::cout << message[i];
  }
  //std::cout << std::endl << std::endl;
}

//BORRAR ESTA FUNCION, ESTA PARA PROBAR LA LOGICA DEL JUEGO
void PeerSocket::add_message(const void* a, int len){
  const char* b = (char*)a;
  for (int i = length; i < length + len; i++) {
    message[i] = b[i];
  }
  length += len;
}


//ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
PeerSocket::PeerSocket(const void* message, int length){
  beginning = 0;
  this->length = length;
  const char* message_stream = (char*)message;
  for (int i = 0; i < length; i++) {
    this->message[i] = message_stream[i];
  }
}
*/

PeerSocket::PeerSocket(int fd){
  //socket_fd = fd;
  set_fd(fd);
}


//CAMBIAR LA IMPLEMENTACION DE ESTE MOVE
PeerSocket::PeerSocket(PeerSocket&& other) noexcept{
  /*
  for (int i = 0; i < other.length; i++) {
    this->message[i] = other.message[i];
  }
  this->length = other.length;
  this->beginning = other.beginning;
  */
  //this->socket_fd = other.socket_fd;
  set_fd(other.socket_fd);
  other.socket_fd = -1;
}

PeerSocket::~PeerSocket(){
  //DO NOTHING
}
