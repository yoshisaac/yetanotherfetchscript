#!/usr/bin/env python3
import sys, os, subprocess

uname = subprocess.check_output("uname -a", shell=True, text=True).split(" ")
whoami = subprocess.check_output("whoami", shell=True, text=True).strip()
cpuinfo = subprocess.check_output("cat /proc/cpuinfo", shell=True, text=True).split("\n")
osinfo = subprocess.check_output("cat /etc/os-release", shell=True, text=True).split("\n")

i = 1
while i < len(sys.argv):
    if sys.argv[i] == "-d" or sys.argv[i] == "--distro":
        osinfo[2] = sys.argv[i+1]
    i = i + 1

osf = open('os.txt', 'r').read().split("\n")
oslogo = [""] * len(osf)

#parse the file that contains the logos
startRead = False
i = 0
for line in osf:
    if line == osinfo[2].replace("ID=", '').strip():
        startRead = True
        continue;

    if line == ":EOF" and startRead == True:
        break;

    if startRead == True:
        oslogo[i] = line.replace("\\033", '\033')
        i = i + 1


#print everything!
print()
print(oslogo[0], "OS:\033[0m", osinfo[0].replace("NAME=", '').replace("\"", ''))
print(oslogo[1], "Kernel:\033[0m", uname[2])
print(oslogo[2], "CPU:\033[0m", uname[12] + " " + cpuinfo[4].replace("model name	: ", '').replace(" CPU", ''))
print(oslogo[3], "User:\033[0m", whoami)
print(oslogo[4], "Host:\033[0m", uname[1])
print(oslogo[5])
print("\033[0m")
