#pragma once

#include <filesystem>
#include <optional>
#include <string_view>

std::optional<std::filesystem::path> find_executable(
    std::string_view command,
    std::string_view path_env);

std::optional<std::filesystem::path> find_executable_in_environment(
    std::string_view command);
