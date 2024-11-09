#pragma once

#include "Global.h"

namespace os {

namespace fs = std::filesystem;

class Shell {
  fs::path current_path;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
  std::unordered_map<string, void (Shell::*)(const std::vector<string>&)> custom_commands;

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
public:
  Shell();

  [[nodiscard]] string getCurrentPath() const;
  [[nodiscard]] double getExecutionTime() const;

  double executeCommandLine(string& line);
  bool startProcess(const char* app_name, char* command_line);

  void exitCommand(const std::vector<string>& args);
  void cdCommand(const std::vector<string>& args);
  void dirCommand(const std::vector<string>& args);

};

}  // namespace os
