import os
import colorama
import argparse

def redPrint(text):
    print(colorama.Fore.RED + text + colorama.Fore.RESET)

def yellowPrint(text):
    print(colorama.Fore.YELLOW + text + colorama.Fore.RESET)

def cmd_sys(args):
    os.system(' '.join(args))
    
def cmd_amake(argsList):
    parser = argparse.ArgumentParser()
    parser.add_argument('platform', nargs=1, choices=['win', 'macos', 'ios', 'android'], help='target platform')
    parser.add_argument('action', nargs=1, choices=['gen', 'build', 'clean', 'ide'], help='operation on generated project file')
    parser.add_argument('-i', '--iset', required=False, default='x64', choices=['x64', 'intel64', 'arm64'], help='target instruction set')
    parser.add_argument('-c', '--config', required=False, default='debug', choices=['debug', 'release'], help='target configuration type')
    parser.add_argument('-t', '--target', required=False, help='specify a target to build')
    args = parser.parse_args(argsList)
    #print(args)
    
    if 'gen' in args.action:
        return cmd_amake_gen(args)
    elif 'build' in args.action:
        return cmd_amake_build(args)
    elif 'clean' in args.action:
        return cmd_amake_clean(args)
    elif 'ide' in args.action:
        return cmd_amake_ide(args)
    else:
        parser.print_help()
    
def cmd_amake_gen(argsMap):
    print('gen')
    return 0
    
def cmd_amake_build(argsMap):
    print('build')
    return 0
    
def cmd_amake_ide(argsMap):
    print('ide')
    return 0
    
def cmd_amake_clean(argsMap):
    print('clean')
    return 0