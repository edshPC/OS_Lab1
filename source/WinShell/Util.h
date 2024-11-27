#pragma once

#include "Global.h"

namespace os::util {

void tolower(std::string& str);
void trim(std::string& str);
std::vector<std::string> split(const std::string& str);
std::vector<std::string> split(string str, const string& delim);
bool contains(const std::string& str, const std::string& substr);

}  // namespace os::util
