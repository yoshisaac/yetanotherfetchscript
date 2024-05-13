#!/usr/bin/env python3
import sys
import os
import subprocess

def get_os_info():
    uname = subprocess.check_output("uname -a", shell=True, text=True).split(" ")
    whoami = subprocess.check_output("whoami", shell=True, text=True).strip()
    cpuinfo = subprocess.check_output("cat /proc/cpuinfo", shell=True, text=True).split("\n")
    osinfo = subprocess.check_output("cat /etc/os-release", shell=True, text=True).split("\n")
    return uname, whoami, cpuinfo, osinfo

def parse_os_logo(osinfo, osf):
    oslogo = [""] * len(osf)
    startRead = False
    i = 0
    for line in osf:
        if line == osinfo[2].replace("ID=", '').strip():
            startRead = True
            continue
        if line == ":EOF" and startRead:
            break
        if startRead:
            oslogo[i] = line.replace("\\033", '\033')
            i += 1
    return oslogo

def main():
    uname, whoami, cpuinfo, osinfo = get_os_info()

    i = 1
    while i < len(sys.argv):
        if sys.argv[i] == "-d" or sys.argv[i] == "--distro":
            osinfo[2] = sys.argv[i+1]
        i += 1

    osf = open('os.txt', 'r').read().split("\n")
    oslogo = parse_os_logo(osinfo, osf)

    print()
    print(oslogo[0], "OS:\033[0m", osinfo[0].replace("NAME=", '').replace("\"", ''))
    print(oslogo[1], "Kernel:\033[0m", uname[2])
    print(oslogo[2], "CPU:\033[0m", uname[12] + " " + cpuinfo[4].replace("model name	: ", '').replace(" CPU", ''))
    print(oslogo[3], "User:\033[0m", whoami)
    print(oslogo[4], "Host:\033[0m", uname[1])
    print(oslogo[5])
    print("\033[0m")

if __name__ == "__main__":
    main()
