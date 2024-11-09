#pragma once

#include "Global.h"

namespace os::util {

void tolower(std::string& str);
void trim(std::string& str);
std::vector<std::string> split(std::string str, const std::string& delimiter = " ");

}  // namespace os::util
