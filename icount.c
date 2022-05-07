#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]) {
  int count = 0;
  int status;
  if (argc != 2) {
    fprintf(stderr, "usage %s ELF executable file.\n", argv[0]);
    return 1;
  }
  fprintf(stderr, "instruction counter start: exec file %s\n", argv[1]);
  int pid = fork();
  if (!pid) {
    // run exe file on child process
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execve(argv[1], argv + 1, envp);
  }
  fprintf(stderr, "PID %d\n", pid);
  while (1) {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
      break;
    ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
    count++;
  }
  fprintf(stderr, "count: %d\n", count);
  return 0;
}
