#include "serverPeerSocket.h"

//BORRAR INCLUDES, ESTO ES PARA EL PROXY
#include <iostream>
#include <string>

///////////////////////////////PUBLIC//////////////////////////

void PeerSocket::receive(void* buffer, int buffer_len){
  char* buff_string = (char*)buffer;
  for (int i = 0; i < buffer_len; i++) {
    buff_string[i] = message[i];
  }
  message += buffer_len;
}


//REIMPLEMENTAR, ES SOLO PARA PROBAR LA LOGICA DEL PROGRAMA
void PeerSocket::send(const void* buffer, int buffer_len){

  std::cout << "MENSAJE ENVIADO POR EL SOCKET" << std::endl;

  char* message = buffer;
  for (size_t i = 0; i < buffer_len; i++) {
    std::cout << message[i];
  }
}


//BORRAR ESTA FUNCION, ESTA PARA PROBAR LA LOGICA DEL JUEGO
void PeerSocket::add_message(void* a, int len){
  char* b = a;
  for (size_t i = length; i < length + len; i++) {
    message[i] = b;
  }
  length += len;
}


//ESTE CONSTRUCTOR ES SOLO PARA PROBAR LA LOGICA DEL JUEGO
PeerSocket::PeerSocket(const void* message, int length){
  this->length = length;
  const char* message_stream = message;
  for (int i = 0; i < length; i++) {
    this->message[i] = message_stream[i];
  }
}

PeerSocket::PeerSocket(PeerSocket&& other) noexcept{
  //IMPLEMENTAR
}

PeerSocket::~PeerSocket(){
  //DO NOTHING
}
