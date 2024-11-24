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
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
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

bool contains(const std::string& str, const std::string& substr) {
  return str.find(substr) != std::string::npos;
}

}  // namespace os::util
