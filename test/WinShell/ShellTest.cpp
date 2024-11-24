#include <gtest/gtest.h>

#include "WinShell/Shell.h"
#include "WinShell/Util.h"

namespace os {

TEST(Shell, ChangeDirectory) { // NOLINT
  Shell shell;

  std::filesystem::create_directory("Test Directory");

  shell.executeCommandLine("cd \"Test Directory\""); // Try to go somewheare
  ASSERT_EQ(util::contains(shell.getCurrentPath(), "Test Directory"), true); // Path must contain Test dir
}

TEST(Shell, Execute) { // NOLINT
  Shell shell;

  shell.executeCommandLine("../benchmark/WinShell-bench-dedup");
  ASSERT_GT(shell.getExecutionTime(), 0); // Exec time must be > 0

  shell.executeCommandLine("tms 4");
  ASSERT_EQ(shell.getProcessCount(), 4); // Set process count
}

}  // namespace os
