#ifndef RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_
#define RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_

#include <string>
#include <vector>

#if defined(_WIN32) && defined(UNICODE)
#define RTW_WIN_UNICODE
#endif  // defined(_WIN32) && defined(UNICODE)

#if defined(RTW_WIN_UNICODE)
#define RTW_SYS_STR_LITERAL(str) L##str
#else
#define RTW_SYS_STR_LITERAL(str) str
#endif

namespace rust_tool_wrapper {

class System {
 public:
#if defined(RTW_WIN_UNICODE)
  using StrType = std::wstring;
#else
  using StrType = std::string;
#endif  // defined(RTW_WIN_UNICODE)

  using Arguments = std::vector<StrType>;
  using EnvironmentBlock = std::vector<StrType>;

 public:
  static StrType ToStrType(const std::string& string);

  static StrType ComposeEnvironmentVariable(const StrType& key,
                                            const StrType& value);

  static StrType GetWorkingDirectory();
  static StrType Join(const StrType& path1, const StrType& path2);

  static int Exec(const StrType& executable, const Arguments& arguments,
                  const EnvironmentBlock& environment_block);

  static int UnTar(const StrType& tar_file, const StrType& out_dir);
};

}  // namespace rust_tool_wrapper

#endif  // RUST_PRIVATE_RUSTC_WRAPPER_SYSTEM_H_
