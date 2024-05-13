#!/usr/bin/env python3
import sys
import os
import subprocess

def get_os_info():
    uname = subprocess.check_output("uname -a", shell=True, text=True).split(" ")
    whoami = subprocess.check_output("whoami", shell=True, text=True).strip()
    arch = subprocess.check_output("uname -m", shell=True, text=True).strip()
    shell = subprocess.check_output("echo $SHELL", shell=True, text=True).strip()
    cpuinfo = subprocess.check_output("cat /proc/cpuinfo", shell=True, text=True).split("\n")
    osrelease = subprocess.check_output("cat /etc/os-release", shell=True, text=True).split("\n")
    return uname, whoami, cpuinfo, osrelease, arch, shell

def parse_os_logo(ID, osf):
    oslogo = [""] * len(osf)
    startRead = False
    i = 0
    for line in osf:
        if line == ID:
            startRead = True
            continue
        if line == ":EOF" and startRead:
            break
        if startRead:
            oslogo[i] = line.replace("\\033", '\033')
            i += 1
    return oslogo

def parse_os_release(osrelease):
    ID = ""
    NAME = ""
    for element in osrelease:
        if "ID=" in element and not "BUILD" in element:
            ID = element.replace("ID=", '').strip()
        if "NAME=" in element and not "PRETTY" in element:
            NAME = element.replace("NAME=", '').replace("\"", '').strip()
    return ID, NAME

def parse_shell(shell):
    if "bash" in shell:
        return "bash"
    elif "zsh" in shell:
        return "zsh"
    else:
        return "Other"

def main():
    uname, whoami, cpuinfo, osrelease, arch, shell = get_os_info()

    ID, NAME = parse_os_release(osrelease)
    shell = parse_shell(shell)

    i = 1
    while i < len(sys.argv):
        if sys.argv[i] == "-d" or sys.argv[i] == "--distro":
            ID = sys.argv[i+1]
        i += 1

    osf = open('os.txt', 'r').read().split("\n")
    oslogo = parse_os_logo(ID, osf)

    print()
    print(oslogo[0], "OS:\033[0m", NAME)
    print(oslogo[1], "Kernel:\033[0m", uname[2])
    print(oslogo[2], "CPU:\033[0m", arch + " " + cpuinfo[4].replace("model name	: ", '').replace(" CPU", ''))
    print(oslogo[3], "Shell:\033[0m", shell)
    print(oslogo[4], "Host:\033[0m", uname[1])
    print(oslogo[5], "User:\033[0m", whoami)
    print("\033[0m")

if __name__ == "__main__":
    main()
