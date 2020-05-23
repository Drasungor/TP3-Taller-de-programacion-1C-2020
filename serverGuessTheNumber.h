#ifndef _SERVER_GUESS_THE_NUMBER_H_
#define  _SERVER_GUESS_THE_NUMBER_H_

#include <fstream>
#include <vector>

class ServerGuessTheNumber {
private:
  void _load_numbers_to_guess(std::ifstream& numbers_file,
                            std::vector<std::string>& numbers_to_guess) const;
public:
  ServerGuessTheNumber();

  ~ServerGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
