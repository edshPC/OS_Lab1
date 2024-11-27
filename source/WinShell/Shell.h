#pragma once

#include "Global.h"

namespace os {

namespace fs = std::filesystem;

class ExitException : public std::exception {};

class Shell {
  fs::path current_path;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
  std::unordered_map<string, void (Shell::*)(const std::vector<string>&)> custom_commands;
  int process_count = 1;

  SECURITY_ATTRIBUTES security_attribute;
  HANDLE std_out_read, std_out_write;
  HANDLE std_in_read, std_in_write;
  bool pipe_stdout, pipe_stdin;

public:
  Shell();

  [[nodiscard]] string getCurrentPath() const;
  [[nodiscard]] double getExecutionTime() const;
  [[nodiscard]] int getProcessCount() const;

  double executeCommandLine(const string& line);
  double executeSingleCommandLine(string line);
  bool startProcess(const char* app_name, char* command_line);

  void exitCommand(const std::vector<string>& args);
  void cdCommand(const std::vector<string>& args);
  void dirCommand(const std::vector<string>& args);
  void tmsCommand(const std::vector<string>& args);

};

}  // namespace os
