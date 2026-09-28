#pragma once

#include <filesystem>
#include <string>
#include <vector>

int run_process(
    const std::filesystem::path& executable,
    const std::vector<std::string>& arguments);
