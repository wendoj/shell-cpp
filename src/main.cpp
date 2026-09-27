#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <sstream>

enum class Command {
  Exit,
  Echo,
  Type,
  Unknown
};

Command parse_command(std::string_view input) {
  const std::size_t separator = input.find(' ');
  const std::string_view name = input.substr(0, separator);

  if (name == "exit") {
    return Command::Exit;
  }
  if (name == "echo") {
    return Command::Echo;
  }
  if (name == "type") {
    return Command::Type;
  }
  return Command::Unknown;
}

bool is_builtin(std::string_view name) {
  static const std::vector<std::string_view> builtins {
      "echo",
      "type",
      "exit"
  };

  return std::find(builtins.begin(), builtins.end(), name) != builtins.end();
}

std::optional<std::filesystem::path> find_executable(
    std::string_view command,
    std::string_view path_env) {
  std::stringstream ss{std::string(path_env)};
  std::string directory;

  constexpr std::filesystem::perms execute_bits =
      std::filesystem::perms::owner_exec |
      std::filesystem::perms::group_exec |
      std::filesystem::perms::others_exec;

  while (std::getline(ss, directory, ':')) {
    const std::filesystem::path candidate_path =
        std::filesystem::path(directory) / std::string(command);

    if (std::filesystem::exists(candidate_path) &&
        (std::filesystem::status(candidate_path).permissions() &
         execute_bits) != std::filesystem::perms::none) {
      return candidate_path;
    }
  }

  return std::nullopt;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    // Get the user's input
    std::string input;
    if (!std::getline(std::cin, input)) {
      break;
    }

    const std::size_t separator = input.find(' ');
    const std::string_view arguments =
        separator == std::string::npos
            ? std::string_view {}
            : std::string_view(input).substr(separator + 1);

    switch (parse_command(input)) {
      case Command::Exit:
        return 0;
      case Command::Echo:
        std::cout << arguments << '\n';
        break;
      case Command::Type:
        if (is_builtin(arguments)) {
          std::cout << arguments << " is a shell builtin\n";
        } else {
          const char* path_env = std::getenv("PATH");
          const std::optional<std::filesystem::path> executable_path =
              path_env == nullptr
                  ? std::nullopt
                  : find_executable(arguments, path_env);

          if (executable_path.has_value()) {
            std::cout << arguments << " is "
                      << executable_path->string() << '\n';
          } else {
            std::cout << arguments << ": not found\n";
          }
        }
        break;
      case Command::Unknown:
        std::cout << input << ": command not found\n";
        break;
    }
  }
}
