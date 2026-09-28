#pragma once

#include "core/command.hpp"

#include <string>
#include <vector>

class Shell {
 public:
  int run();

 private:
  void handle_echo(const std::vector<std::string>& arguments) const;
  void handle_type(const std::vector<std::string>& arguments) const;
  static void handle_external(const ParsedCommand& command);
  static void handle_pwd();

  static constexpr std::string_view builtins[] = {"echo", "type", "pwd", "exit"};
};
