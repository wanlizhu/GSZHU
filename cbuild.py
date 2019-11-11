import os
import getpass
import socket
import colorama
import inspect
import argparse
import traceback
import platform


RESET   = colorama.Fore.RESET   + colorama.Back.RESET + colorama.Style.RESET_ALL
RED_INV = colorama.Style.BRIGHT + colorama.Fore.WHITE + colorama.Back.RED
RED     = colorama.Style.BRIGHT + colorama.Back.RESET + colorama.Fore.RED
CYAN    = colorama.Style.BRIGHT + colorama.Back.RESET + colorama.Fore.CYAN
GREEN   = colorama.Style.BRIGHT + colorama.Back.RESET + colorama.Fore.GREEN
YELLOW  = colorama.Style.BRIGHT + colorama.Back.RESET + colorama.Fore.YELLOW
MAGENTA = colorama.Style.BRIGHT + colorama.Back.RESET + colorama.Fore.MAGENTA


class ArgumentParserNoInterrupt(argparse.ArgumentParser):
    def error(self, message):
        raise Exception('error: ' + message)


class Argument:
    def __init__(self, name, type='string'):
        self.type = type
        self.name = name

    def of(self, namespace):
        if hasattr(namespace, self.name):
            return namespace.name
        return [] if self.type=='list' else ''

class CBuild:
    """Handle all CMake build/install commands"""
    source_dir = os.path.dirname(__file__)
    binary_dir = os.path.join(source_dir, 'build')

    def __init__(self):
        pass

    def print_status(self):
        os.system('git branch -vv')
        os.system('git status -uno')
        print('\n')

    def run_command(self, args):
        cmd = args[0].replace('-', '_')
        if not (hasattr(CBuild, cmd) and callable(getattr(CBuild, cmd))):
            print(RED, "'%s' is not recognized as an internal CBuild command,\n" % (cmd), 
                  YELLOW, "run 'cbuild --help' for more information.\n", RESET)
            return
        return getattr(CBuild, cmd)(self, args)

    def conan_install(self, args):
        os.system('conan install --build miss --install-folder "%s" "%s"' % (CBuild.binary_dir, CBuild.source_dir))
        print('\n')

    def gen(self, args):
        try:
            iswindows = platform.system().startswith('Win')
            parser = ArgumentParserNoInterrupt()
            parser.add_argument('-p', '--platform', 
                                required=False,
                                default='win' if iswindows else 'macos', 
                                choices=['win', 'macos', 'ios', 'android'], 
                                help='target platform, e.g. android/ios')
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
            parser.add_argument('-d', '--define', 
                                required=False,
                                default='',
                                action='append',
                                help='add CMake definitions, e.g. BUILD_SHARED_LIBS:BOOL=ON')
            namespace = parser.parse_args(args)
        except Exception as error:
            print(RED_INV, repr(error), RESET, end='\n\n')
            traceback.print_exc()
            return 1

        cmake_gen = ['cmake']
        cmake_gen += ['-S', '%s'%(CBuild.source_dir)]
        cmake_gen += ['-B', '%s'%(CBuild.binary_dir)]
        cmake_gen += ['-G', '%s'%('Visual Studio 16 2019 -A %i'%(namespace.iset) if iswindows else 'Xcode')]
        cmake_gen += ['-D' + var for var in Argument('define', 'list').of(namespace)]
        print(CYAN, ' '.join(cmake_gen), RESET)
        os.system(' '.join(cmake_gen))
        return 0

    def ide(self, args):
        os.system('cmake --open "%s"' % (CBuild.binary_dir))
        return 0

    def build(self, args):
        parser = ArgumentParserNoInterrupt()
        parser.add_argument('-t', '--target', 
                            required=False, 
                            default='',  
                            help='specify a target to build, otherwise build all targets')
        parser.add_argument('-c', '--config', 
                            required=False, 
                            default='debug',  
                            help='build debug or release')
        parser.add_argument('-cf', '--clean-first', 
                            required=False, 
                            action='store_true',  
                            help='Build target "clean" first, then build. \n(To clean only, use --target "clean".)')
        namespace = parser.parse_args(args)
        
        cmake_build = ['cmake', '--build']
        cmake_build += ['%s' % (CBuild.binary_dir)]
        cmake_build += ['--target %s' % (namespace.target) if hasattr(namespace, 'target') else '']
        cmake_build += ['--config %s' % (namespace.config) if hasattr(namespace, 'config') else ''] 
        cmake_build += ['--clean-first' if hasattr(namespace, 'clean_first') else ''] 
        print(CYAN, ' '.join(cmake_build), RESET)
        os.system(' '.join(cmake_build))
        return 0

if __name__=='__main__':
    cbuild = CBuild()
    cbuild.print_status()

    while True:
        print(GREEN, getpass.getuser() + '@' + socket.gethostname() + '$ ', RESET, end='')
        parts = input().split()
        if parts[0]=='cbuild': 
            cbuild.run_command(parts[1:])
        else:
            os.system(' '.join(parts))
    exit(0)
