#include <fstream>
#include <iostream>
#include <regex>
#include <streambuf>
#include <string>
#include <unordered_map>
#include <vector>

#include "rust/private/rust_tool_wrapper/system.h"

// Simple rustc wrapper allowing us to prepare the context to call
// Since it's only used as a private implementation detail of a rule and not
// user invoked we don't bother with error checking.
#if defined(RTW_WIN_UNICODE)
int wmain(int argc, const wchar_t* argv[], const wchar_t* envp[]) {
#else
int main(int argc, const char* argv[], const char* envp[]) {
#endif  // defined(RTW_WIN_UNICODE)
  using namespace rust_tool_wrapper;

  // Parse args.
  System::StrType tool_path;
  System::StrType out_dir;
  System::StrType tar_file;
  System::StrType rename_from;
  System::StrType rename_to;
  System::StrType maker_path;
  System::Arguments arguments;
  System::EnvironmentBlock environment_block;
  for (int i = 1; i < argc; ++i) {
    System::StrType arg = argv[i];
    if (arg == RTW_SYS_STR_LITERAL("--tool-path")) {
      tool_path = argv[++i];
    } else if (arg == RTW_SYS_STR_LITERAL("--out-dir")) {
      out_dir = System::JoinWithWorkingDirectory(arg);
      System::ComposeEnvironmentVariable(RTW_SYS_STR_LITERAL("OUT_DIR"),
                                         out_dir);

    } else if (arg == RTW_SYS_STR_LITERAL("--build-env-file")) {
      std::ifstream env_file(argv[++i]);
      std::string line;
      while (std::getline(env_file, line)) {
        environment_block.push_back(System::ToStrType(line));
      }
    } else if (arg == RTW_SYS_STR_LITERAL("--build-flags-file")) {
      std::ifstream env_file(argv[++i]);
      std::string line;
      while (std::getline(env_file, line)) {
        // replace <EXEC_ROOT> by the exec root
        System::StrType sys_line = System::ToStrType(line);
        std::size_t pos = sys_line.find(RTW_SYS_STR_LITERAL("<EXEC_ROOT>"));
        if (pos != std::string::npos) {
          sys_line.replace(pos, 11, System::GetWorkingDirectory());
        }
        arguments.push_back(sys_line);
      }
    } else if (arg == RTW_SYS_STR_LITERAL("--package-dir")) {
      System::ComposeEnvironmentVariable(
          RTW_SYS_STR_LITERAL("CARGO_MANIFEST_DIR"),
          System::JoinWithWorkingDirectory(argv[++i]));
    } else if (arg == RTW_SYS_STR_LITERAL("--maker-path")) {
      maker_path = argv[++i];
    } else if (arg == RTW_SYS_STR_LITERAL("--rename")) {
      rename_from = argv[++i];
      rename_to = argv[++i];
    } else if (arg == RTW_SYS_STR_LITERAL("--")) {
      for (++i; i < argc; ++i) {
        arguments.push_back(argv[i]);
      }
    }
  }

  for (int i = 0; envp[i] != nullptr; ++i) {
    environment_block.push_back(envp[i]);
  }
  int exit_code = System::Exec(tool_path, arguments, environment_block);
  if (exit_code == 0) {
    if (!maker_path.empty()) {
      std::ofstream file(maker_path);
    }
    if (!rename_from.empty() && !rename_to.empty()) {
      std::ifstream source(rename_from, std::ios::binary);
      std::ofstream dest(rename_to, std::ios::binary);
      dest << source.rdbuf();
    }
  }
  return exit_code;
}
