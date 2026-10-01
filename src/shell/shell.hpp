#pragma once

#include "core/command.hpp"

#include <string>
#include <vector>

class Shell {
 public:
  int run();

 private:
  static void handle_echo(const std::vector<std::string>& arguments);
  static void handle_type(const std::vector<std::string>& arguments);
  static void handle_external(const ParsedCommand& command);
  static void handle_pwd();
  static void handle_cd(const std::vector<std::string>& arguments);

  static constexpr std::string_view builtins[] = {"echo", "type", "pwd", "cd", "exit"};
};
