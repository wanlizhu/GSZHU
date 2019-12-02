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


iswindows = platform.system().startswith('Win')
vsname = '"Visual Studio 16 2019" -A x64'
cmd = 'cmake -S . -B ./build -G %s -D GE_CODE_REVIEW_MODE:BOOL=ON' % (vsname if iswindows else 'Xcode')
runcommand(cmd)

cmd = 'cmake --open ./build'
runcommand(cmd)