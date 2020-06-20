#include <fstream>
#include <regex>
#include <streambuf>
#include <string>
#include <vector>

#include "rust/private/rustc_wrapper/third_party/microtar/src/microtar.h"
#include "rust/private/rustc_wrapper/third_party/tiny-process-library/process.hpp"

// Simple rustc wrapper allowing us to prepare the context to call
// C++ in an abstract way
// and outputs the result.
//
// To invoke:
// expand_template
// --template PATH
// --output PATH
// regex0;replacement0
// regex1;replacement1
// ...
//
// Since it's only used as a private implementation detail of a rule and not
// user invoked we don't bother with error checking.
int main(int argc, const char** argv) {
  // Parse args.
  const char* rustc_path = nullptr;
  const char* rename_from = nullptr;
  const char* rename_to = nullptr;
  for (int i = 1; i < argc; ++i) {
    const char* arg = argv[i];
    if (strcmp(arg, "--rustc") == 0) {
      rustc_path = argv[++i];
    } else if (strcmp(arg, "--rename") == 0) {
      rename_from = argv[++i];
      rename_to = argv[++i];
    } else {
      
    }
  }

  //// Read template.
  //std::ifstream ifs(template_path);
  //std::string str(std::istreambuf_iterator<char>(ifs),
  //                (std::istreambuf_iterator<char>()));
//
  //// Apply regexes.
  //for (const auto& subst : substitutions) {
  //  str = std::regex_replace(str, subst.regex, subst.replacement);
  //}
//
  //// Output file.
  //std::ofstream file(output_path);
  //file << str;

  return 0;
}
