#include "system/process.hpp"

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int run_process(
    const std::filesystem::path& executable,
    const std::vector<std::string>& arguments) {
  std::vector<char*> argv;
  argv.reserve(arguments.size() + 1);
  for (const std::string& argument : arguments) {
    argv.push_back(const_cast<char*>(argument.c_str()));
  }
  argv.push_back(nullptr);

  const pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "fork failed\n";
    return 1;
  }

  if (pid == 0) {
    execv(executable.c_str(), argv.data());
    std::cerr << arguments.front() << ": failed to execute\n";
    _exit(126);
  }

  int status = 0;
  if (waitpid(pid, &status, 0) == -1) {
    std::cerr << "waitpid failed\n";
    return 1;
  }

  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  }
  return 1;
}
