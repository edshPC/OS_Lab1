#include "Shell.h"
#include "Util.h"

static const UINT CpWin1251 = 1251;

int main() {
  os::Shell shell;
  std::string input;

  SetConsoleOutputCP(CpWin1251);
  SetConsoleCP(CpWin1251);

  while (true) {
    std::cout << shell.getCurrentPath() << "> ";
    if (!std::getline(std::cin, input)) { break; }
    try {
      if (const double exec_time = shell.executeCommandLine(input); exec_time > 0) {
        std::cout << "Process executed in " << exec_time << "s\n";
      }
    } catch (const os::ExitException&) {
      break;
    } catch (const std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }

  return 0;
}
