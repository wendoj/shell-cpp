#include <iostream>
#include <string>
#include <functional>

enum class Command {
  exit,
  echo,
  type,
  unknown
};

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
      break;
    } else if (input.substr(0, 5) == "echo ") {
      std::cout << input.substr(5) << '\n';
    } else if (input.substr(0, 5) == "type ") {
      std::string type { input.substr(5) };

      if (type == "echo" || type == "type" || type == "exit") {
        std::cout << type << " is a shell builtin\n";
      } else {
        std::cout << type << ": not found\n";
      }
    }
    else {
      std::cout << input << ": command not found\n";
    }
  }
}
