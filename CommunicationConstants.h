#ifndef _COMMUNICATION_CONSTANTS_H_
#define  _COMMUNICATION_CONSTANTS_H_

//Header for the constants that both the client and the server need to know
//for proper communication and functionality

#define NUMBERS_DIGITS_AMMOUNT 3

#define LOSE_MESSAGE "Perdiste\n"
#define WIN_MESSAGE "Ganaste\n"

enum CommandIndicator:char {
  COMMAND_INDICATOR_HELP = 'h', COMMAND_INDICATOR_GIVE_UP = 's',
  COMMAND_INDICATOR_NUMBER = 'n'
};

#endif
