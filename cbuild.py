import os
import sys
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
    def exit(self, status=0, message=None):
        if status!=0:
            return self.error('error(%i): %s' % (status, message))

    def error(self, message):
        print(RED_INV, message, RESET)


class Argument:
    def __init__(self, name, type='string'):
        self.type = type
        self.name = name

    def of(self, namespace):
        if hasattr(namespace, self.name):
            return getattr(namespace, self.name)
        return [] if self.type=='list' else ''

class CBuild:
    """Handle all CMake build/install commands"""
    source_dir = os.path.dirname(__file__)
    binary_dir = os.path.join(source_dir, 'build')

    @staticmethod
    def alias():
        return 'cb'

    def __init__(self):
        pass

    def __help(self):
        methods = [name.replace('_', '-') for name in dir(self) 
                                              if callable(getattr(self, name)) and 
                                                 not name.startswith('_')]
        print(MAGENTA, 'Available subcommands of cbuild:')
        for method in methods:
            print(CYAN, '\t%s' % (method))
        print(RESET)

    def print_status(self):
        os.system('git branch -vv')
        os.system('git status -uno')

    def run_command(self, subcmd, args):
        if subcmd in ['-h', '--help']:
            return self.__help()

        method = subcmd.replace('-', '_')
        if not (hasattr(self, method) and callable(getattr(self, method))):
            print(RED, "'%s' is not recognized as an internal CBuild command,\n" % (subcmd), 
                  YELLOW, "run 'cbuild --help' for more information.\n", RESET)
            return
        return getattr(self, method)(args)

    def init_packages(self, args):
        parser = ArgumentParserNoInterrupt()
        parser.add_argument('-rb', '--rebuild', 
                            required=False, 
                            default=False,
                            action='store_true',  
                            help='Rebuild all from sources, do not use, do not use binary packages')
        namespace = parser.parse_args(args)
        
        conan_args = ['conan', 'install']
        conan_args += ['--build', '' if namespace.rebuild else 'missing']
        conan_args += ['--install-folder', '"%s"' % (CBuild.binary_dir)]
        conan_args += ['"%s"' % (CBuild.source_dir)]
        print(CYAN, '>>', ' '.join(conan_args), RESET)
        os.system(' '.join(conan_args))

    def gen(self, args):
        if not os.path.exists(CBuild.binary_dir):
            self.init_packages([])
            
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

        vsname = '"Visual Studio 16 2019" -A %s' % (namespace.iset)
        cmake_gen = ['cmake']
        cmake_gen += ['-S', '"%s"'%(CBuild.source_dir)]
        cmake_gen += ['-B', '"%s"'%(CBuild.binary_dir)]
        cmake_gen += ['-G', '%s'%(vsname if iswindows else 'Xcode')]
        cmake_gen += ['-D' + var for var in Argument('define', 'list').of(namespace)]
        print(CYAN, '>>', ' '.join(cmake_gen), RESET)
        os.system(' '.join(cmake_gen))
        return 0

    def ide(self, args):
        os.system('cmake --open "%s"' % (CBuild.binary_dir))
        return 0
        
    def clean(self, args):
        os.system('cmake -E remove_directory "%s"' % (CBuild.binary_dir))
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
        print(CYAN, '>>', ' '.join(cmake_build), RESET)
        os.system(' '.join(cmake_build))
        return 0


def await_input():
    print(GREEN, getpass.getuser() + '@' + socket.gethostname() + '$', RESET, end='')
    line = input()
    words = line.split()
    cmd = words[0] if len(words) >= 1 else ''
    subcmd = words[1] if len(words) >= 2 else ''
    args = words[2:] if len(words) >= 3 else []
    return cmd, subcmd, args

if __name__=='__main__':
    thismodule = sys.modules[__name__]
    classes = [info[0] for info in inspect.getmembers(thismodule, 
                                                      lambda member: inspect.isclass(member) and 
                                                                     member.__module__==__name__)]
    cbuild = CBuild()
    cbuild.print_status()
    cbuild.init_packages([])
    
    while True:
        cmd, subcmd, args = await_input()
        instance = None
        for name in classes:
            thisclass = getattr(thismodule, name)
            if not cmd==name.lower():
                if (not hasattr(thisclass, 'alias')) or (not cmd==thisclass.alias()):
                    continue
            instance = thisclass()
            instance.run_command(subcmd, args)
            break
        if not instance:
            os.system('%s %s %s' % (cmd, subcmd, ' '.join(args)))
            continue
    exit(0)
