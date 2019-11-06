import json
import os
import sys
import re
import importlib
import importlib.util
import inspect
import getpass
import socket
import colorama
import traceback

def redBackText(text):
    return colorama.Back.RED + colorama.Fore.WHITE + text + colorama.Fore.RESET + colorama.Back.RESET

def redText(text):
    return colorama.Fore.RED + text + colorama.Fore.RESET

def greenText(text):
    return colorama.Fore.GREEN + text + colorama.Fore.RESET

def blueText(text):
    return colorama.Fore.BLUE + text + colorama.Fore.RESET

def yellowText(text):
    return colorama.Fore.YELLOW + text + colorama.Fore.RESET

def unpackVariable(var):
    translator = {
        'PWD': os.path.abspath(os.getcwd()),
    }
    
    match = re.search(r'(\$ENV\{[a-z|A-Z|0-9|_]+\})|(\$\{[a-z|A-Z|0-9|_]+\})', var)
    if match:
        expr = match.group()
        if expr.startswith('$ENV{') and expr.endswith('}'):
            exprValue = os.environ[expr[5:-1]]
        elif expr.startswith('${') and expr.endswith('}'):
            exprValue = translator.get(expr[2:-1], None)
        var = var.replace(expr, exprValue if exprValue is not None else '', 1)

        return unpackVariable(var)
    else:
        return var
    

def initEnv():
    colorama.init(autoreset=True)
    envJson = 'init_env.json'
    with open(envJson) as jsonFile:
        data = json.load(jsonFile)
        for key, value in data.items():
            key = unpackVariable(key)
            value = unpackVariable(value)
            os.environ[key] = value

def inputCommand():
    prefix = getpass.getuser() + '@' + socket.gethostname()
    print(greenText(prefix) + '$ ', end='')
    return input()

def prepareCommand(cmdLine):
    cmdLine = cmdLine.strip()
    parts = cmdLine.split()
    cmdName = parts[0]
    cmdArgs = parts[1:]
    return cmdName, cmdArgs

def findCommand(cmdName, scriptsPath):
    cmdFuncName = 'cmd_' + cmdName if '-' not in cmdName else cmdName.replace('-', '_')
    for entry in os.scandir(scriptsPath):
        if entry.is_dir() or entry.name=='__init__.py' or \
                             os.path.splitext(entry.path)[1] not in ['.py', '.pyc']:
            continue

        spec = importlib.util.spec_from_file_location('devcmd', entry.path)
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)

        for obj in inspect.getmembers(module, inspect.isfunction):
            if obj[0] == cmdFuncName:
                return getattr(module, obj[0])
    return None


def dispatchCommand(cmdName, cmdArgs):
    shouldQuit = True if cmdName in ['quit', 'exit'] else False
    if not shouldQuit:
        cmdFunc = findCommand(cmdName, os.environ['SCRIPTS_DIR'])
        if cmdFunc is not None:
            try:
                cmdFunc(cmdArgs)
            except Exception as error:
                print(redText(str(error)))
        else: # fallback to run it as system command
            print(yellowText('[forward to system command]'))
            os.system(cmdName + ' ' + ' '.join(cmdArgs))
    return shouldQuit

if __name__=='__main__':
    initEnv()
    shouldQuit = False
    cmdName = None
    cmdArgs = []
    try:
        while not shouldQuit:
            cmdLine = inputCommand()
            if not cmdLine:
                continue
            cmdName, cmdArgs = prepareCommand(cmdLine)
            shouldQuit = dispatchCommand(cmdName, cmdArgs)
    except Exception as error:
        print(redBackText('Exception'), redText(str(error)))
        traceback.print_exc()
        os.system('pause')
        exit(1)
    exit(0)
