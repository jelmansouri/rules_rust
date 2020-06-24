#include "rust/private/rust_tool_wrapper/system.h"

//

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>

namespace rust_tool_wrapper {

System::StrType System::GetWorkingDirectory() {
  const DWORD kMaxBufferLength = 4096;
  char cwd[kMaxBufferLength];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    return System::StrType{};
  }
  return System::StrType{cwd};
}

System::StrType System::JoinWithWorkingDirectory(const StrType &relative_path) {
  return GetWorkingDirectory() + "/" + relative_path;
}

int System::Exec(const System::StrType &executable,
                 const System::Arguments &arguments,
                 const System::EnvironmentBlock &environment_block) {
  id_type child_pid = fork();
  if (child_pid < 0) {
    return -1;
  } else if (child_pid == 0) {
    std::vector<const char *> argv;
    for (const StrType &argument : arguments) {
      argv.push_back(argument.c_str());
    }
    argv.push_back(nullptr);

    std::vector<const char *> envp;
    for (const StrType &ev : environment_block) {
      envp.push_back(ev.c_str());
    }
    envp.push_back(nullptr);

    umask(022);

    execve(executable.c_str(), argv.data(), envp.data());
    return -1;
  }

  int err, exit_status = -1;
  do {
    err = waitpid(child_pid, &exit_status, 0);
  } while (err == -1 && errno == EINTR);

  return exit_status;
}

}  // namespace rust_tool_wrapper