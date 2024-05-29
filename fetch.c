#include <sys/utsname.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//for matching a section of a string, not the entire strings
bool match(const char* stra, const char* strb) {
  for (unsigned int i = 0; i < strlen(strb); ++i) {
    if (stra[i] == '\0') return false;
    
    if (stra[i] != strb[i])
      return false;
  }

  return true;
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

const char* calc_mhz(const char* mhz_str) {
  bool is_ghz = strlen(mhz_str) >= 4;

  char* ret_mhz = malloc(strlen(mhz_str)+4);

  unsigned int i;
  if (!is_ghz) {
    for (i = 0; i < strlen(mhz_str); ++i) {
      ret_mhz[i] = mhz_str[i];
    }
    ret_mhz[ strlen(mhz_str) ] = ' ';
    ret_mhz[strlen(mhz_str)+1] = 'M';
    ret_mhz[strlen(mhz_str)+2] = 'H';
    ret_mhz[strlen(mhz_str)+3] = 'z';
    ret_mhz[strlen(mhz_str)+4] = '\0';
  } else if (is_ghz) {
    bool start_bump = false;
    char tmp;
    for (i = 0; i < strlen(mhz_str)-1; ++i) {
      if (i == 2) { tmp = mhz_str[i]; ret_mhz[i] = '.'; start_bump = true; continue; }
      if (start_bump) {
	ret_mhz[i] = tmp;
	tmp = mhz_str[i+1];
	continue;
      }
      ret_mhz[i] = mhz_str[i];
    }
    ret_mhz[ strlen(mhz_str) ] = ' ';
    ret_mhz[strlen(mhz_str)+1] = 'G';
    ret_mhz[strlen(mhz_str)+2] = 'H';
    ret_mhz[strlen(mhz_str)+3] = 'z';
    ret_mhz[strlen(mhz_str)+4] = '\0';
  }

  return ret_mhz;
}

const char* get_cpu(void) {
  FILE* cpuinfo = fopen("/proc/cpuinfo", "r");

  //if your cpu name is more than 256 characters, then you've got your own issues
  char* cpu         = malloc(256);
  char* cpu_mhz     = malloc(256);
  char* line        = malloc(256);
  bool past_colon   = false;
  bool on_modelname = false;
  bool on_cpumhz    = false;
  int cpu_t         = 0;
  int cpu_mhz_t     = 0;
  int line_t        = 0;
  int ch;
  while ((ch = fgetc(cpuinfo)) != EOF) {
    if (ch == '\n') { past_colon   = false;
                      on_modelname = false;
		      on_cpumhz    = false;
		      line_t       = 0;
		      memset(line, '\0', 256);
		      continue; }

    if (ch == ':')  { past_colon   = true;  continue; } // skip the colon
    line[line_t] = ch;
    if (match(line, "power management")) break;

    if (match(line, "model name")) on_modelname = true;
    if (on_modelname && past_colon) {
      if (ch == '@') { cpu[cpu_t]   = '@';
	               cpu[++cpu_t] = ' ';
		       past_colon   = false;
		       continue; }
      cpu[cpu_t] = ch;
      ++cpu_t;
    }

    if (match(line, "cpu MHz")) on_cpumhz = true;
    if (on_cpumhz && past_colon) {
      if (ch == '.') { past_colon = false; continue; }
      cpu_mhz[cpu_mhz_t] = ch;
      ++cpu_mhz_t;
    }

    ++line_t;
  }

  const char* mhz = calc_mhz(cpu_mhz);
  unsigned int i = 0;
  for (i; i < strlen(mhz); ++i) {
    cpu[cpu_t+i] = mhz[i];
  }
  cpu[cpu_t+i] = '\0';

  //remove unwanted strings
  for (unsigned int i = 0; i < strlen(cpu); ++i) {
    if (match(cpu+i, "CPU"))
      for (unsigned int h = 0; h < 4; ++h) cpu[i+h] = 6;

    if (match(cpu+i, "Processor"))
      for (unsigned int h = 0; h < 9; ++h) cpu[i+h] = 6;
  }

  free(line);
  
  return cpu;
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
