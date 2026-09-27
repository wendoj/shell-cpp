#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";

  // Get the user's input
  std::string input;
  std::getline(std::cin, input);

  std::cout << input << ": command not found" << "\n";
}
