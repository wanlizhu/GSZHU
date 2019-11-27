import os
import sys
import getpass
import socket
import colorama
import inspect
import argparse
import traceback
import platform

iswindows = platform.system().startswith('Win')
vsname = '"Visual Studio 16 2019" -A x64'
cmake_gen = ['cmake']
cmake_gen += ['-S', '"."']
cmake_gen += ['-B', '"./build"']
cmake_gen += ['-G', '%s'%(vsname if iswindows else 'Xcode')]
cmake_gen += ['-D', 'GE_CODE_REVIEW_MODE:BOOL=OFF']
cmd = ' '.join(cmake_gen)
print('>>', cmd)
os.system(cmd)

cmd = 'cmake --build ./build --target Editor --parallel'
print('>>', cmd)
os.system(cmd)

cmd = 'cd ./build/bin && ./Editor'
print('>>', cmd)
os.system(cmd)