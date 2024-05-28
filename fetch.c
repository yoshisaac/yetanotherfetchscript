#include <sys/utsname.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool match(const char* stra, const char* strb) {

}

const char* get_term(void) {
  const char* terminal = getenv("TERM");
  if (!terminal) return "unknown";

  if (strcmp(terminal, "xterm-256color") == 0)
    return "xterm";
  
  if (strcmp(terminal, "xterm-kitty") == 0)
    return "kitty";
  
  return terminal;
}

const char* get_shell(void) {
  const char* shell = getenv("SHELL");
  if (!shell) return "unknown";

  if (strcmp(shell, "/bin/bash") == 0)
    return "bash";
  
  return shell;
}

enum CPUINFO {
  PROCESSOR,
  VENDOR_ID,
  CPU_FAMILY,
  MODEL,
  MODEL_NAME,
  STEPPING,
  MICROCODE,
  CPU_MHZ,
  CACHE_SIZE,
  PHYSICAL_ID,
  SIBLINGS,
  CORE_ID,
  CPU_CORES,
  APICID,
  INITIAL_APICID,
  FPU,
  FPU_EXCEPTION,
  CPUID_LEVEL,
  WP,
  FLAGS,
  VMX_FLAGS,
  BUGS,
  BOGOMIPS,
  CLFLUSH_SIZE,
  CACHE_ALIGNMENT,
  ADDRESS_SIZES,
  POWER_MANAGEMENT,
  EO_CPU
};

const char* get_cpu(void) {
  FILE* cpuinfo = fopen("/proc/cpuinfo", "r");

  //if your cpu name is more than 256 characters, then you've got your own issues
  char* cpu_name  = malloc(256); 
  int line        = 0;
  bool past_colon = false;
  int i           = 0;
  int ch;
  while ((ch = fgetc(cpuinfo)) != EOF) {
    if (ch == '\n')    { past_colon  = false; line++;   } // track which line we're at
    if (ch == ':')     { past_colon  = true;  continue; } // skip the colon
    if (line == EO_CPU) break;

    if (line == MODEL_NAME && past_colon == true) {
      cpu_name[i] = ch;
      i++;
    }
  }
  cpu_name[i] = '\0';

  for (unsigned int i = 0; i < strlen(cpu_name); ++i) {
    if (cpu_name[i] == 'C' && cpu_name[i+1] == 'P' && cpu_name[i+2] == 'U')
      for (unsigned int h = 0; h < 4; ++h) {
	cpu_name[i+h] = 6; //acknowledge that we don't want that character
      }
  }
  
  return cpu_name;
}

int main(void) {
  struct utsname name;
  if (uname(&name) == -1) return 1;

  struct passwd* pw = getpwuid(geteuid());
  if (!pw) return 1;
  
  printf("Kernel: %s\n", name.release);
  printf("Shell: %s\n", get_shell());
  printf("CPU: %s%s\n", name.machine, get_cpu());
  printf("Host: %s\n", name.nodename);
  printf("User: %s\n", pw->pw_name);
  printf("Term: %s\n", get_term());
  
  return 0;
}
