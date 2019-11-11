import os
import getpass
import socket
import colorama
import inspect

def reset_color():
    return colorama.Fore.RESET + colorama.Back.RESET + colorama.Style.RESET_ALL

def fore_color(color):
    return colorama.Style.BRIGHT + colorama.Back.RESET + color

def red(text):
    return fore_color(colorama.Fore.RED) + text + reset_color()

def cyan(text):
    return fore_color(colorama.Fore.CYAN) + text + reset_color()

def green(text):
    return fore_color(colorama.Fore.GREEN) + text + reset_color()

def yellow(text):
    return fore_color(colorama.Fore.YELLOW) + text + reset_color()

def magenta(text):
    return fore_color(colorama.Fore.MAGENTA) + text + reset_color()


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
            print(red("'%s' is not recognized as an internal CBuild command,\n" % (cmd)), 
                  yellow("run 'cbuild --help' for more information."))
            return
        return getattr(CBuild, cmd)(self, args)

    def conan_install(self, args):
        os.system('conan install --install-folder "%s" "%s"' % (CBuild.binary_dir, CBuild.source_dir))
        print('\n')
        pass
        

if __name__=='__main__':
    cbuild = CBuild()
    cbuild.print_status()

    while True:
        print(green(getpass.getuser() + '@' + socket.gethostname()) + '$ ', end='')
        parts = input().split()
        if parts[0]=='cbuild': 
            cbuild.run_command(parts[1:])
        else:
            os.system(' '.join(parts))
    exit(0)
