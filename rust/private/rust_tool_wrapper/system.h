#ifndef RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_
#define RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_

#include <string>
#include <unordered_map>

#if defined(_WIN32) && defined(UNICODE)
#define RTW_WIN_UNICODE
#endif  // defined(_WIN32) && defined(UNICODE)

namespace rust_tool_wrapper {

#if defined(RTW_WIN_UNICODE)
#define RTW_SYS_STR_LITERAL(str) L##str
#else
#define RTW_SYS_STR_LITERAL(str) str
#endif

class System {
 public:
#if defined(RTW_WIN_UNICODE)
  using StrType = std::wstring;
#else
  using StrType = std::string;
#endif  // defined(_WIN32)

  using Arguments = std::vector<StrType>;
  using EnvironmentBlock = std::vector<StrType>;

  static StrType ComposeEnvironmentVariable(const StrType& key,
                                            const StrType& value) {
    StrType env = key;
    env.push_back(RTW_SYS_STR_LITERAL('='));
    env += (value);
    return env;
  }

  static StrType GetWorkingDirectory();
  static StrType JoinWithWorkingDirectory(const StrType& relative_path);

  static int Exec(const System::StrType& executable, const Arguments& arguments,
                  const EnvironmentBlock& environment_block);
};

}  // namespace rust_tool_wrapper

#endif  // RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_
