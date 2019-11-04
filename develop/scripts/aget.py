import os
import colorama
import argparse

def redPrint(text):
    print(colorama.Fore.RED + text + colorama.Fore.RESET)

def yellowPrint(text):
    print(colorama.Fore.YELLOW + text + colorama.Fore.RESET)
    
def cmd_aget(argsList):
    parser = argparse.ArgumentParser()
    parser.add_argument('platform', nargs=1, choices=['win', 'macos', 'ios', 'android'], help='target platform')
    parser.add_argument('-i', '--iset', required=False, default='x64', choices=['x64', 'intel64', 'arm64'], help='target instruction set')
    parser.add_argument('-c', '--config', required=False, default='debug', choices=['debug', 'release'], help='target configuration type')
    args = parser.parse_args(argsList)
    #print(args)
    

def download_package(url, local):
    pass
    
def generate_package(local, buildPath)
    pass
    
def build_package(buildPath):
    pass
    
def install_package(buildPath, dest):
    pass