#pragma once

#include "Global.h"

namespace os {

namespace fs = std::filesystem;

class Shell {
  fs::path current_path;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
public:
  Shell();

  void cd(const string& path);

  double executeCommandLine(string& line);
  bool startProcess(const char* app_name, char* command_line);

  [[nodiscard]] string getCurrentPath() const;
  [[nodiscard]] double getExecutionTime() const;

};

}  // namespace os
