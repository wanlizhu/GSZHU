import os
import colorama
import argparse
import json
import shutil
import sys
import subprocess
import traceback

__json = None
__cmdArgs = None
__runtimeName = None

def redPrint(text):
    print(colorama.Fore.RED + text + colorama.Fore.RESET)

def yellowPrint(text):
    print(colorama.Fore.YELLOW + text + colorama.Fore.RESET)

def execute_command(cmdLine, cwd=None):
    if cwd:
        cmdLine = 'cd "%s" && '%(cwd) + cmdLine

    print('\n', cmdLine, '\n')
    ec = os.system(cmdLine)
    if not ec == 0:
        redPrint('error(%d): failed to run "%s"' % (ec, cmdLine))
        return False
    return True

def cmake_generator_name():
    global __json
    global __cmdArgs
    global __runtimeName

    if __cmdArgs.platform in ['macos', 'ios']:
        return 'Xcode'

    # for Windows host platform
    result = subprocess.run(['cmake', '--help'], 
                            stdout=subprocess.PIPE, 
                            shell=False)
    cmakeHelp = result.stdout.decode('utf-8')

    # get the arch name of target platform
    versionList = ['16 2019', '15 2017', '14 2015']
    for name in ['Visual Studio ' + v for v in versionList]:
        if name not in cmakeHelp:
            continue
        name = '"' + name + '"'
        return name + (' -A ' if int(name[-5:-1]) >= 2019 else ' ') + __cmdArgs.iset
    
    yellowPrint('error: no required cmake generator was found')
    return ''

class ArgumentParserNoInterrupt(argparse.ArgumentParser):
    def error(self, message):
        raise Exception('error: ' + message)

def cmd_aget(argsList):
    global __json
    global __cmdArgs
    global __runtimeName

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
        __runtimeName = '-'.join([__cmdArgs.platform[0], __cmdArgs.iset, __cmdArgs.config])
        #print(__cmdArgs)
    except Exception as error:
        redPrint(str(error))
        traceback.print_exc()
        return 1

    # load packages.json which contains information to download and build packages
    with open('packages.json') as jsonFile:
        __json = json.load(jsonFile)
        if 'CACHE_DIR' not in __json:
            __json['CACHE_DIR'] = '../__cache__'
        if 'INSTALL_DIR' not in __json:
            __json['INSTALL_DIR'] = 'packages'
        if 'DISABLE_LIST' not in __json:
            __json['DISABLE_LIST'] = []

    for info in __json['PACKAGE_LIST']:
        if info['NAME'] in __json['DISABLE_LIST']:
            continue
        succeed, sourceDir = download_package(info)
        buildDir = sourceDir + '-build'
        if not succeed:
            continue
        if not generate_package(info, sourceDir):
            continue
        if not install_package(info, buildDir):
            continue
    return 0

def download_package(info):
    global __json
    global __cmdArgs
    global __runtimeName

    successful = False
    sourceDir = os.path.join(__json['CACHE_DIR'], __runtimeName, info['NAME'], info['COMMIT_ID'])

    if os.path.exists(sourceDir) and os.path.exists(sourceDir + '/.git'):
        cmdLine1 = 'git reset --hard && git fetch --tags'
        cmdLine2 = 'git checkout %s' % (info['COMMIT_ID'])
        successful = execute_command(cmdLine1, cwd=sourceDir)
        successful = execute_command(cmdLine2, cwd=sourceDir)
    else:
        cmdLine = ' '.join([info['COMMAND'], info['URL'], sourceDir])
        successful = execute_command(cmdLine)
          
    return (successful, sourceDir)
    
def generate_package(info, sourceDir):
    global __json
    global __cmdArgs
    global __runtimeName

    generatorName = cmake_generator_name()
    buildDir = sourceDir + '-build'
    cmakeDefines = ['-D CMAKE_INSTALL_PREFIX="%s"' % (__json['INSTALL_DIR'] + '/' + info['NAME'])]
    cmakeDefines += ['-D ' + v for v in (info['CMAKE_DEFINES'] if 'CMAKE_DEFINES' in info else [])]
    
    cmdLine = ' '.join(['cmake', 
                        '-S "%s"'%(sourceDir), 
                        '-B "%s"'%(buildDir),
                        '-G %s'%(generatorName) if generatorName else '',
                        *cmakeDefines])
    return execute_command(cmdLine)
    
def install_package(info, buildDir):
    global __json
    global __cmdArgs
    global __runtimeName

    return execute_command('cmake --install "%s" --config %s' % (buildDir, __cmdArgs.config))