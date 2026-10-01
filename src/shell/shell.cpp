#include "shell/shell.hpp"

#include "core/command.hpp"
#include "system/path_search.hpp"
#include "system/process.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

void Shell::handle_echo(const std::vector<std::string>& arguments) {
  for (std::size_t index = 0; index < arguments.size(); ++index) {
    if (index > 0) {
      std::cout << ' ';
    }
    std::cout << arguments[index];
  }
  std::cout << '\n';
}

void Shell::handle_type(const std::vector<std::string>& arguments) {
  if (arguments.empty()) {
    std::cout << "type: missing argument\n";
    return;
  }

  const std::string& command = arguments.front();
  const bool is_builtin =
      std::find(std::begin(builtins), std::end(builtins), command) !=
      std::end(builtins);

  if (is_builtin) {
    std::cout << command << " is a shell builtin\n";
    return;
  }

  const auto executable = find_executable_in_environment(command);
  if (executable.has_value()) {
    std::cout << command << " is " << executable->string() << '\n';
  } else {
    std::cout << command << ": not found\n";
  }
}

void Shell::handle_external(const ParsedCommand& command) {
  if (command.name.empty()) {
    return;
  }

  const auto executable =
      find_executable_in_environment(command.name);
  if (!executable.has_value()) {
    std::cout << command.name << ": command not found\n";
    return;
  }

  std::vector<std::string> process_arguments;
  process_arguments.reserve(command.arguments.size());
  process_arguments.push_back(command.name);
  process_arguments.insert(process_arguments.end(), command.arguments.begin(),
                           command.arguments.end());
  run_process(*executable, process_arguments);
}

void Shell::handle_pwd() {
  std::cout << std::getenv("PWD") << "\n";
}

void Shell::handle_cd(const std::vector<std::string> &arguments) {
  std::filesystem::path directory_path = arguments.front();

  if (std::filesystem::exists(directory_path)) {
    std::filesystem::current_path(directory_path);
  } else {
    std::cout << "cd: " << directory_path.string() << ": No such file or directory\n";
  }
}

int Shell::run() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    if (!std::getline(std::cin, input)) {
      return 0;
    }

    const ParsedCommand command = parse_command(input);
    switch (command.type) {
      case CommandType::Exit:
        return 0;
      case CommandType::Echo:
        handle_echo(command.arguments);
        break;
      case CommandType::Type:
        handle_type(command.arguments);
        break;
      case CommandType::Pwd:
        handle_pwd();
        break;
      case CommandType::Cd:
        handle_cd(command.arguments);
        break;
      case CommandType::External:
        handle_external(command);
        break;
    }
  }
}
