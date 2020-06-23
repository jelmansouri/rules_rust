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
#if defined(RUSTC_WRAPPER_WIN_UNICODE)
// wmain should work in MingW when linking with -municode
// but was not tested
int wmain(int argc, const wchar_t* argv[], const wchar_t* envp[]) {
#else
int main(int argc, const char* argv[], const char* envp[]) {
#endif  // defined(_WIN32)
  using namespace rust_tool_wrapper;

  // Parse args.
  System::StrType rustc_path;
  System::StrType rename_from;
  System::StrType rename_to;
  System::Arguments arguments;
  for (int i = 1; i < argc; ++i) {
    System::StrType arg = argv[i];
    if (arg == RUSTC_WRAPPER_SYS_STR_LITERAL("--tool-path")) {
      rustc_path = argv[++i];
    } else if (arg == RUSTC_WRAPPER_SYS_STR_LITERAL("--rename")) {
      rename_from = argv[++i];
      rename_to = argv[++i];
    } else if (arg == RUSTC_WRAPPER_SYS_STR_LITERAL("--")) {
      for (++i; i < argc; ++i) {
        arguments.push_back(argv[i]);
      }
    }
  }
  std::wcout << "Testing: " << System::GetWorkingDirectory() << std::endl;
  System::EnvironmentBlock environment_block;
  for (int i = 0; envp[i] != nullptr; ++i) {
    environment_block.push_back(envp[i]);
  }
  return System::Exec(rustc_path, arguments, environment_block);

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
