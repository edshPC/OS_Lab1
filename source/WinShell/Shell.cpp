#include "Shell.h"

#include "Util.h"

#define CHECK_ARG_NUMBER(num) if (args.size() != num) {\
  throw std::invalid_argument(std::format("Wrong number of arguments: {}. Expected: {}", args.size(), num).c_str()); }

namespace os {

Shell::Shell() : current_path(std::filesystem::current_path()), si(), pi() {
  custom_commands["exit"] = &Shell::exitCommand;
  custom_commands["cd"] = &Shell::cdCommand;
  custom_commands["dir"] = &Shell::dirCommand;
}

string Shell::getCurrentPath() const {
  return current_path.string();
}

double Shell::getExecutionTime() const {
  return std::chrono::duration<double>(end_time - start_time).count();
}

double Shell::executeCommandLine(string& line) {
  auto args = util::split(line);
  if (args.empty()) {
    return 0;
  }
  string& cmd = args[0];
  if (custom_commands.contains(cmd)) {
    (this->*custom_commands.at(cmd))(args);
    return 0;
  }

  if(!util::contains(cmd, ".")) {
    cmd += ".exe";
  }
  const char* file_path = (current_path / cmd).string().c_str();
  char* c_line = line.data();

  if (!startProcess(file_path, c_line) && !startProcess(nullptr, c_line)) {
    throw std::exception(std::format("Failed to start process: {}", GetLastError()).c_str());
  }
  return getExecutionTime();
}

bool Shell::startProcess(const char* app_name, char* command_line) {
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  start_time = std::chrono::high_resolution_clock::now();
  if (!CreateProcess(
          app_name,                       // имя исполняемого файла
          command_line,                   // командная строка
          nullptr,                        // процесс не наследует дескрипторы
          nullptr,                        // поток не наследует дескрипторы
          FALSE,                          // не наследовать дескрипторы
          0,                              // флаги создания
          nullptr,                        // использовать текущую среду
          current_path.string().c_str(),  // использовать текущую директорию
          &si,                            // информация о старте
          &pi                             // информация о процессе
      )) {
    return false;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);
  end_time = std::chrono::high_resolution_clock::now();

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return true;
}

void Shell::exitCommand(const std::vector<string>& args) {
  CHECK_ARG_NUMBER(1);
  exit(0);
}

void Shell::cdCommand(const std::vector<string>& args) {
  CHECK_ARG_NUMBER(2);
  auto new_path = fs::weakly_canonical(current_path / args[1]);
  if (!fs::exists(new_path)) {
    throw std::invalid_argument("No such directory");
  }
  if (!fs::is_directory(new_path)) {
    throw std::invalid_argument("Path is not a directory");
  }
  current_path = new_path;
}
void Shell::dirCommand(const std::vector<string>& args) {
  CHECK_ARG_NUMBER(1);
  for (const auto& entry : fs::directory_iterator(current_path)) {
    std::cout << entry.path().filename().string() << std::endl;
  }
}

}  // namespace os
