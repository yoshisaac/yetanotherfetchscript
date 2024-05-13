#!/usr/bin/env python3
import sys, os, subprocess
"""
i = 0
while i < len(sys.argv):
    if sys.argv[i]
    i = i + 1
"""
osf = open('os.txt', 'r').read().split("\n")
oslogo = [str] * len(osf)

uname = subprocess.check_output("uname -a", shell=True, text=True).split(" ")
cpuinfo = subprocess.check_output("cat /proc/cpuinfo", shell=True, text=True).split("\n")
osinfo = subprocess.check_output("cat /etc/os-release", shell=True, text=True).split("\n")

startRead = False
i = 0
for line in osf:
    
    if line == osinfo[2].replace("ID=", ''):        
        startRead = True
        continue;

    if line == ":":
        break;

    if startRead == True:
        oslogo[i] = line.replace("\\033", '\033')
        i = i + 1



print()
print(oslogo[0], "OS:\033[0m", osinfo[0].replace("NAME=", '').replace("\"", ''))
print(oslogo[1], "Kernel:\033[0m", uname[2])
print(oslogo[2], "CPU:\033[0m", uname[12] + " " + cpuinfo[4].replace("model name	: ", '').replace(" CPU", ''))
print(oslogo[3], "User:\033[0m", uname[1])
print(oslogo[4])
print(oslogo[5])
print("\033[0m")
