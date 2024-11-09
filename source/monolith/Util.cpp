#include "Util.h"

namespace os::util {

void tolower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
}
void trim(std::string& str) {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

std::vector<std::string> split(std::string str, const std::string& delimiter) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    if (std::string const token = str.substr(0, pos); !token.empty()) {
      tokens.push_back(token);
    }
    str.erase(0, pos + delimiter.length());
  }
  if (!str.empty()) {
    tokens.push_back(str);
  }
  return tokens;
}

}  // namespace os::util
