#include "serverGuessTheNumber.h"

#include <fstream>
#include <vector>
#include "CommunicationConstants.h"

#define SUCCESS 0
#define INVALID_FILE 1
#define WRONG_NUMBER_OF_ARGUMENTS 1

#define NUMBER_OF_ARGUMENTS 3
#define NUMBERS_FILE_ARGUMENTS_INDEX 2
#define PORT_ARGUMENTS_INDEX 1



//AGREGAR DESCRIPCION DE LO QUE HACE LA FUNCION, AGREGAR CADA CASO DE EXCEPTION
//PORQUE SOLO SE DESCRIBE LA EXCEPTION TIRADA EN UN CASO PARTICULAR
//If a read string does not represent a number and is also of an undesired
//length, the same exception as the one with undesired length exception will
//be thrown VER SI ESTO ULTIMO HAY QUE CAMBIARLO, CREO QUE EL ENUNCIADO
//CONSIDERA EL CASO EN EL QUE EL ARCHIVO DE ENTRADA TIENE STRINGS QUE NO SON
//NUMEROS
void ServerGuessTheNumber::_load_numbers_to_guess(
                                  std::ifstream& numbers_file,

                                  //std::vector<int>& numbers_to_guess){
                                  std::vector<std::string>& numbers_to_guess) const{
  std::string buffer;
  std::getline(numbers_file, buffer);
  //DESCOMENTAR ESTO
  //int number;
  while (!numbers_file.eof()) {
    if (buffer.length() != NUMBERS_DIGITS_AMMOUNT) {
      //TIRAR LA EXCEPCION ADECUADA
    }

    //HACER UN TRY CATCH DEL STOI PARA VER SI EL NUMERO ES O NO UN NUMERO
    //DESCOMENTAR ESTO
    //number = std::stoi(buffer);

    //numbers_to_guess.push_back(number);
    numbers_to_guess.push_back(buffer);
    std::getline(numbers_file, buffer);
  }
}


///////////////////////////////PUBLIC//////////////////////////

int ServerGuessTheNumber::execute(const char** arguments, int number_of_arguments){


  //VER SI SE PASA TODO LO QUE PUEDE DEVOLVER UNA EXCEPCION A UNA FUNCION Y
  //TENER UN TRY CATCH QUE AGARRE TODAS LAS EXCEPCIONES POSIBLES E IMPRIMA UN
  //MENSAJE DE ERROR PARA CADA UNA
  if (number_of_arguments != NUMBER_OF_ARGUMENTS) {
    //AGREGAR PRINT DE MENSAJE DE ERROR
    return WRONG_NUMBER_OF_ARGUMENTS;
  }


  std::ifstream numbers_file(arguments[NUMBERS_FILE_ARGUMENTS_INDEX]);

  //VER SI SE PASA TODO LO QUE PUEDE DEVOLVER UNA EXCEPCION A UNA FUNCION Y
  //TENER UN TRY CATCH QUE AGARRE TODAS LAS EXCEPCIONES POSIBLES E IMPRIMA UN
  //MENSAJE DE ERROR PARA CADA UNA
  if (!numbers_file.is_open()) {
    //AGREGAR PRINT DE MENSAJE DE ERROR
    return INVALID_FILE;
  }
  //FALTA AGREGAR CODIGO RELACIONADO CON EL PORT

  std::vector<std::string> numbers_to_guess;
  //bool keep_processing = true;
  _load_numbers_to_guess(numbers_file, numbers_to_guess);

  /*
  while (keep_processing) {
    keep_processing = ;
  }
  */
  //AGREGAR CODIGOS DE RETORNO DE ERROR

  return SUCCESS;
}


ServerGuessTheNumber::ServerGuessTheNumber(){
}


ServerGuessTheNumber::~ServerGuessTheNumber(){
}
