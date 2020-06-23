#ifndef RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_
#define RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_

#include <string>
#include <unordered_map>

#if defined(_WIN32) && defined(UNICODE)
#define RUSTC_WRAPPER_WIN_UNICODE
#endif  // defined(_WIN32) && defined(UNICODE)

namespace rust_tool_wrapper {

#if defined(RUSTC_WRAPPER_WIN_UNICODE)
#define RUSTC_WRAPPER_SYS_STR_LITERAL(str) L##str
#else
#define RUSTC_WRAPPER_SYS_STR_LITERAL(str) str
#endif

class System {
 public:
#if defined(RUSTC_WRAPPER_WIN_UNICODE)
  using StrType = std::wstring;
#else
  using StrType = std::string;
#endif  // defined(_WIN32)

  using Arguments = std::vector<StrType>;
  using EnvironmentBlock = std::vector<StrType>;

  static StrType GetWorkingDirectory();
  static StrType GetJoinWithWorkingDirectory(const StrType& relative_path);

  static int Exec(const System::StrType& executable, const Arguments& arguments,
                  const EnvironmentBlock& environment_block);
};

}  // namespace rust_tool_wrapper

#endif  // RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_