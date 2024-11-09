#include "Shell.h"
#include "Util.h"

int main() {
  os::Shell shell;
  std::string input;

  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  while (true) {
    std::cout << shell.getCurrentPath() << "> ";
    std::getline(std::cin, input);
    os::util::trim(input);
    try {
      if (const double exec_time = shell.executeCommandLine(input); exec_time > 0) {
        std::cout << "Process executed in " << exec_time << "s" << std::endl;
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }

  return 0;
}
