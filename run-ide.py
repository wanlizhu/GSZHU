import os
import sys
import getpass
import socket
import colorama
import inspect
import argparse
import traceback
import platform

def runcommand(cmd):
    print('>>', cmd)
    if not os.system(cmd) == 0:
        os.system('pause')


cmd = 'conan remove --locks'
runcommand(cmd)

cmd = 'conan install . --build missing --install-folder ./build'
runcommand(cmd)

iswindows = platform.system().startswith('Win')
vsname = '"Visual Studio 16 2019" -A x64'
cmd = 'cmake -S . -B ./build -G %s' % (vsname if iswindows else 'Xcode')
runcommand(cmd)

cmd = 'cmake --open ./build'
runcommand(cmd)