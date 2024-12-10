#include <iostream>

#include "CommandEvaluator.h"
#include "UnknownCommandException.h"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  auto evaluator = CommandEvaluator();
  while (true) {
    std::cout << "$ ";
    try {
      std::string input;
      std::getline(std::cin, input);
      if(!input.empty()) {
        if(evaluator.isValidCommand(input)) {
          std::cout << "Command recnorized: " << input << std::endl;
        } else {
          throw UnknownCommandException(input);
        }
      }
    } catch (UnknownCommandException & err) {
      std::cout << err.what() << std::endl;
    }
  }
}
