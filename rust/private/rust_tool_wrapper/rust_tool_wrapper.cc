#include <fstream>
#include <iostream>
#include <regex>
#include <streambuf>
#include <string>
#include <unordered_map>
#include <vector>

#include "rust/private/rust_tool_wrapper/system.h"
#include "third_party/microtar/src/microtar.h"

// Simple rustc wrapper allowing us to prepare the context to call
// Since it's only used as a private implementation detail of a rule and not
// user invoked we don't bother with error checking.
#if defined(RTW_WIN_UNICODE)
int wmain(int argc, const wchar_t* argv[], const wchar_t* envp[]) {
#else
int main(int argc, const char* argv[], const char* envp[]) {
#endif  // defined(_WIN32)
  using namespace rust_tool_wrapper;

  // Parse args.
  System::StrType tool_path;
  System::StrType out_dir;
  System::StrType tar_file;
  System::StrType build_env_file;
  std::vector<System::StrType> build_flags_files;
  System::StrType package_dir;
  System::StrType rename_from;
  System::StrType rename_to;
  System::StrType maker_path;
  System::Arguments arguments;
  for (int i = 1; i < argc; ++i) {
    System::StrType arg = argv[i];
    if (arg == RTW_SYS_STR_LITERAL("--tool-path")) {
      tool_path = argv[++i];
    } else if (arg == RTW_SYS_STR_LITERAL("--out-dir")) {
      out_dir = arg;
    } else if (arg == RTW_SYS_STR_LITERAL("--tar-file")) {
      tar_file = arg;
    } else if (arg == RTW_SYS_STR_LITERAL("--build-env-file")) {
      build_env_file = arg;
    } else if (arg == RTW_SYS_STR_LITERAL("--build-flags-file")) {
      build_flags_files.push_back(arg);
    } else if (arg == RTW_SYS_STR_LITERAL("--package-dir")) {
      package_dir = arg;
    } else if (arg == RTW_SYS_STR_LITERAL("--maker-path")) {
      maker_path = arg;
    } else if (arg == RTW_SYS_STR_LITERAL("--rename")) {
      rename_from = argv[++i];
      rename_to = argv[++i];
    } else if (arg == RTW_SYS_STR_LITERAL("--")) {
      for (++i; i < argc; ++i) {
        arguments.push_back(argv[i]);
      }
    }
  }

  System::EnvironmentBlock environment_block;
  System::ComposeEnvironmentVariable(
      RTW_SYS_STR_LITERAL("CARGO_MANIFEST_DIR"),
      System::JoinWithWorkingDirectory(package_dir));
  if (!out_dir.empty()) {
    System::ComposeEnvironmentVariable(
        RTW_SYS_STR_LITERAL("OUT_DIR"),
        System::JoinWithWorkingDirectory(out_dir));
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
    }
  }
  return exit_code;
  //// Read template.
  // std::ifstream ifs(template_path);
  // std::string str(std::istreambuf_iterator<char>(ifs),
  //                (std::istreambuf_iterator<char>()));
  //
  //// Apply regexes.
  // for (const auto& subst : substitutions) {
  //  str = std::regex_replace(str, subst.regex, subst.replacement);
  //}
  //
  //// Output file.
  // std::ofstream file(output_path);
  // file << str;
}
