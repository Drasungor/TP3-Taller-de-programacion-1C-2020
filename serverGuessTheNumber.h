#ifndef _SERVER_GUESS_THE_NUMBER_H_
#define  _SERVER_GUESS_THE_NUMBER_H_

#include <fstream>
#include <vector>

class ServerGuessTheNumber {
private:
  bool _is_number(const std::string& number_string) const;
  bool _has_repeated_digits(const std::string& number_string) const;
  void _load_numbers_to_guess(std::ifstream& numbers_file,
                            std::vector<std::string>& numbers_to_guess) const;
public:
  ServerGuessTheNumber();

  ~ServerGuessTheNumber();

  int execute(const char** arguments, int number_of_arguments);
};

#endif
