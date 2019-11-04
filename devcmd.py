import json
import os
import sys
import re
import importlib
import importlib.util
import inspect
import getpass
import socket

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
    envJson = 'init_env.json'
    with open(envJson) as jsonFile:
        data = json.load(jsonFile)
        for key, value in data.items():
            key = unpackVariable(key)
            value = unpackVariable(value)
            os.environ[key] = value

def inputCommand():
    print(getpass.getuser() + '@' + socket.gethostname() + '$ ', end='')
    return input()

def prepareCommand(cmdLine):
    cmdLine = cmdLine.strip()
    parts = cmdLine.split()
    cmdName = parts[0]
    cmdArgs = parts[1:]
    return cmdName, cmdArgs

def findCommand(cmdName, scriptsPath):
    for entry in os.scandir(scriptsPath):
        if entry.is_dir() or entry.name=='__init__.py' or \
                             os.path.splitext(entry.path)[1] not in ['.py', '.pyc']:
            continue

        spec = importlib.util.spec_from_file_location('devcmd', entry.path)
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)

        for obj in inspect.getmembers(module, inspect.isfunction):
            if obj[0] == ('cmd_' + cmdName):
                return getattr(module, obj[0])

    return None


def dispatchCommand(cmdName, cmdArgs):
    shouldQuit = True if cmdName in ['quit', 'exit'] else False
    if not shouldQuit:
        cmdFunc = findCommand(cmdName, os.environ['SCRIPTS_PATH'])
        if cmdFunc is not None:
            cmdFunc(cmdArgs)
        else: # fallback to run it as system command
            os.system(cmdName + ' ' + ' '.join(cmdArgs))

    return shouldQuit

if __name__=='__main__':
    initEnv()
    shouldQuit = False
    cmdName = None
    cmdArgs = []
    while not shouldQuit:
        cmdLine = inputCommand()
        if not cmdLine:
            continue

        cmdName, cmdArgs = prepareCommand(cmdLine)
        shouldQuit = dispatchCommand(cmdName, cmdArgs)
        
