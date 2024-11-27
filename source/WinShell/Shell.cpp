#include "Shell.h"

#include "Util.h"

#define CHECK_ARG_NUMBER(num)                                                                \
  if (args.size() != num) {                                                                  \
    throw std::invalid_argument(                                                             \
        std::format("Wrong number of arguments: {}. Expected: {}", args.size(), num).c_str() \
    );                                                                                       \
  }

namespace os {

Shell::Shell() : current_path(std::filesystem::current_path()) {
  custom_commands["exit"] = &Shell::exitCommand;
  custom_commands["cd"] = &Shell::cdCommand;
  custom_commands["dir"] = &Shell::dirCommand;
  custom_commands["tms"] = &Shell::tmsCommand;
}

string Shell::getCurrentPath() const {
  return current_path.string();
}
double Shell::getExecutionTime() const {
  return std::chrono::duration<double>(end_time - start_time).count();
}
int Shell::getProcessCount() const {
  return process_count;
}

double Shell::executeCommandLine(string line) {
  auto lines = util::split(line, "|");
  auto args = util::split(line);
  if (args.empty()) {
    return 0;
  }
  string& cmd = args[0];
  if (custom_commands.contains(cmd)) {
    (this->*custom_commands.at(cmd))(args);
    return 0;
  }

  if (!util::contains(cmd, ".")) {
    cmd += ".exe";
  }
  const string file_path = (current_path / cmd).string();
  const char* c_file_path = file_path.c_str();
  char* c_line = line.data();

  if (!startProcess(c_file_path, c_line) && !startProcess(nullptr, c_line)) {
    throw std::exception(std::format("Failed to start process: {}", GetLastError()).c_str());
  }
  return getExecutionTime();
}

bool Shell::startProcess(const char* app_name, char* command_line) {
  STARTUPINFO startup_info;
  ZeroMemory(&startup_info, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);

  std::vector<PROCESS_INFORMATION> process_informations(process_count);
  std::vector<HANDLE> process_handles(process_count);

  start_time = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < process_count; i++) {
    auto* p_info = &process_informations[i];
    ZeroMemory(p_info, sizeof(PROCESS_INFORMATION));
    if (!CreateProcess(
            app_name,                       // имя исполняемого файла
            command_line,                   // командная строка
            nullptr,                        // процесс не наследует дескрипторы
            nullptr,                        // поток не наследует дескрипторы
            FALSE,                          // не наследовать дескрипторы
            0,                              // флаги создания
            nullptr,                        // использовать текущую среду
            current_path.string().c_str(),  // использовать текущую директорию
            &startup_info,                  // информация о старте
            p_info                          // информация о процессе
        )) {
      for (int j = 0; j < i; j++) {  // prevent zombie
        CloseHandle(process_informations[j].hProcess);
        CloseHandle(process_informations[j].hThread);
      }
      return false;
    }
    process_handles[i] = p_info->hProcess;
  }

  WaitForMultipleObjects(process_count, process_handles.data(), TRUE, INFINITE);
  end_time = std::chrono::high_resolution_clock::now();

  for (auto& pi : process_informations) {
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  return true;
}

void Shell::exitCommand(const std::vector<string>& args) {  // NOLINT(*-convert-member-functions-to-static)
  CHECK_ARG_NUMBER(1);
  throw ExitException();
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
    std::cout << entry.path().filename().string() << '\n';
  }
}
void Shell::tmsCommand(const std::vector<string>& args) {
  CHECK_ARG_NUMBER(2);
  process_count = std::stoi(args[1]);
  std::cout << "Creation process count is set to " << process_count << '\n';
}

}  // namespace os
