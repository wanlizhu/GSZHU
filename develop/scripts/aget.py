import os
import colorama
import argparse
import json
import shutil
import sys
import subprocess
import traceback

__packagesJson = None
__cmdArgs = None

def redPrint(text):
    print(colorama.Fore.RED + text + colorama.Fore.RESET)

def yellowPrint(text):
    print(colorama.Fore.YELLOW + text + colorama.Fore.RESET)

class ArgumentParserNoInterrupt(argparse.ArgumentParser):
    def error(self, message):
        raise Exception('error: ' + message)

def cmd_aget(argsList):
    global __packagesJson
    global __cmdArgs
    try:
        parser = ArgumentParserNoInterrupt()
        parser.add_argument('platform', 
                            nargs=1, 
                            choices=['win', 'macos', 'ios', 'android'], 
                            help='target platform')
        parser.add_argument('-i', '--iset', 
                            required=False, 
                            default='x64', 
                            choices=['x64', 'intel64', 'arm64'], 
                            help='target instruction set')
        parser.add_argument('-c', '--config', 
                            required=False, 
                            default='debug', 
                            choices=['debug', 'release'], 
                            help='target configuration type')
        __cmdArgs = parser.parse_args(argsList)
        #print(__cmdArgs)
    except Exception as error:
        redPrint(str(error))
        traceback.print_exc()
        return 1

    # load packages.json which contains information to download and build packages
    with open(os.path.join(os.environ['ENGINE_DIR'], 'packages.json')) as jsonFile:
        __packagesJson = json.load(jsonFile)

    for info in __packagesJson['PACKAGE_LIST']:
        if not download_package(info):
            continue
        if not generate_package(info):
            continue
        if not build_package(info):
            continue
        if not install_package(info):
            continue
    return 0

def execute_command(cmdLine, cwd=None):
    if cwd:
        cmdLine = 'cd "%s" && '%(cwd) + cmdLine

    print(cmdLine)
    ec = os.system(cmdLine)
    if not ec == 0:
        redPrint('error(%d): failed to run "%s"' % (ec, cmdLine))
        return False
    return True

def cmake_generator_name():
    global __packagesJson
    global __cmdArgs
    if __cmdArgs.platform in ['macos', 'ios']:
        return 'Xcode'

    # for Windows host platform
    result = subprocess.run(['cmake', '--help'], 
                            stdout=subprocess.PIPE, 
                            shell=False)
    cmakeHelp = result.stdout.decode('utf-8')

    # get the arch name of target platform
    prefix = 'Visual Studio'
    suffixList = ['16 2019', '15 2017', '14 2015']
    for name in [prefix + ' ' + suffix for suffix in suffixList]:
        if name not in cmakeHelp:
            continue
        name = '"' + name + '"'
        return name + (' -A ' if int(name[-5:-1]) >= 2019 else ' ') + __cmdArgs.iset
    
    yellowPrint('error: no required cmake generator was found')
    return ''

def download_package(info):
    global __packagesJson
    global __cmdArgs

    cacheDir = os.path.join(__packagesJson['CACHE_DIR'], info['NAME'])
    if os.path.exists(cacheDir):
        execute_command('git reset --hard && git fetch', cwd=cacheDir)
        return execute_command('git checkout %s' % (info['COMMIT_ID']), cwd=cacheDir)
          
    cmdLine = ' '.join([info['COMMAND'], 
                             info['URL'], 
                             os.path.join(cacheDir, info['NAME'])])
    return execute_command(cmdLine)
    
def generate_package(info):
    global __packagesJson
    global __cmdArgs

    cacheDir = __packagesJson['CACHE_DIR']
    generatorName = cmake_generator_name()
    sourceDir = os.path.join(cacheDir, info['NAME']) 
    buildDir = os.path.join(sourceDir, 'build')
    defineList = info['CMAKE_DEFINITIONS'] if 'CMAKE_DEFINITIONS' in info else []
    defineLine = ('-D ' if defineList else '') + (' -D '.join(defineList) if defineList else '')
    cmdLine = ' '.join(['cmake', 
                             '-S "%s"'%(sourceDir), 
                             '-B "%s"'%(buildDir),
                             '-G %s'%(generatorName) if generatorName else '',
                             defineLine])
    return execute_command(cmdLine)
    
def build_package(info):
    global __packagesJson
    global __cmdArgs
    return True
    
def install_package(info):
    global __packagesJson
    global __cmdArgs
    return True