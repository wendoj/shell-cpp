#pragma once

#include <string>
#include <string_view>
#include <vector>

enum class CommandType {
  Exit,
  Echo,
  Type,
  Pwd,
  Cd,
  External,
};

struct ParsedCommand {
  CommandType type;
  std::string name;
  std::vector<std::string> arguments;
};

ParsedCommand parse_command(std::string_view input);
