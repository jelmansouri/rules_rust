#include "rust/private/rust_tool_wrapper/system.h"

namespace rust_tool_wrapper {

System::StrType System::ToStrType(const std::string& string) {
#if defined(RTW_WIN_UNICODE)
  return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(string);
#else
  return string;
#endif  // defined(RTW_WIN_UNICODE)
}

System::StrType System::ComposeEnvironmentVariable(const StrType& key,
                                                   const StrType& value) {
  StrType env = key;
  env.push_back(RTW_SYS_STR_LITERAL('='));
  env += (value);
  return env;
}

}  // namespace rust_tool_wrapper
