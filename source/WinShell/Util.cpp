#include "Util.h"

namespace os::util {

void tolower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) {
    return std::tolower(ch);
  });
}
void trim(std::string& str) {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
              return !std::isspace(ch);
            }));
  str.erase(
      std::find_if(
          str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }
      ).base(),
      str.end()
  );
}

std::vector<std::string> split(const std::string& str) {
  std::istringstream iss(str);
  std::vector<std::string> res;
  std::string current;
  while (iss >> std::quoted(current)) {
    res.push_back(current);
  }
  return res;
}
std::vector<std::string> split(string str, const string& delim) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  while ((pos = str.find(delim)) != std::string::npos) {
    if (const string token = str.substr(0, pos); !token.empty()) {
      tokens.push_back(token);
    }
    str.erase(0, pos + delim.length());
  }
  if (!str.empty()) {
    tokens.push_back(str);
  }
  return tokens;
}

bool contains(const std::string& str, const std::string& substr) {
  return str.find(substr) != std::string::npos;
}

}  // namespace os::util
