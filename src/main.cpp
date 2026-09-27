#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    // Get the user's input
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit") {
      return 0;
    }

    std::cout << input << ": command not found" << "\n";
  }
}
