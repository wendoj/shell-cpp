#include "core/command.hpp"

#include <sstream>
#include <utility>

ParsedCommand parse_command(std::string_view input) {
  std::istringstream input_stream{std::string(input)};
  std::vector<std::string> words;
  std::string word;

  while (input_stream >> word) {
    words.push_back(word);
  }

  if (words.empty()) {
    return {CommandType::External, {}, {}};
  }

  const std::string name = words.front();
  CommandType type = CommandType::External;
  if (name == "exit") {
    type = CommandType::Exit;
  } else if (name == "echo") {
    type = CommandType::Echo;
  } else if (name == "type") {
    type = CommandType::Type;
  } else if (name == "pwd") {
    type = CommandType::Pwd;
  }

  words.erase(words.begin());
  return {type, name, std::move(words)};
}
