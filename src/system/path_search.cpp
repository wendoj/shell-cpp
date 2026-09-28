#include "system/path_search.hpp"

#include <cstdlib>
#include <sstream>
#include <string>

std::optional<std::filesystem::path> find_executable(
    std::string_view command,
    std::string_view path_env) {
  std::stringstream path_stream{std::string(path_env)};
  std::string directory;

  constexpr std::filesystem::perms execute_bits =
      std::filesystem::perms::owner_exec |
      std::filesystem::perms::group_exec |
      std::filesystem::perms::others_exec;

  while (std::getline(path_stream, directory, ':')) {
    const std::filesystem::path candidate =
        std::filesystem::path(directory) / std::string(command);

    if (std::filesystem::exists(candidate) &&
        (std::filesystem::status(candidate).permissions() & execute_bits) !=
            std::filesystem::perms::none) {
      return candidate;
    }
  }

  return std::nullopt;
}

std::optional<std::filesystem::path> find_executable_in_environment(
    std::string_view command) {
  const char* path_env = std::getenv("PATH");
  if (path_env == nullptr) {
    return std::nullopt;
  }

  return find_executable(command, path_env);
}
