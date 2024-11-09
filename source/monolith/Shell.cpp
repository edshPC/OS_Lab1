#include "Shell.h"

#include "Util.h"

namespace os {

Shell::Shell() : current_path(std::filesystem::current_path()) {
}

void Shell::cd(const string& path) {
  auto new_path = fs::weakly_canonical(current_path / path);
  if (!fs::exists(new_path)) {
    throw std::invalid_argument("No such directory");
  }
  if (!fs::is_directory(new_path)) {
    throw std::invalid_argument("Path is not a directory");
  }
  current_path = new_path;
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
  const string& cmd = args[0];
  if (cmd == "exit") {
    exit(0);
  }
  if (cmd == "cd" && args.size() > 1) {
    cd(args[1]);
    return 0;
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

}  // namespace os
