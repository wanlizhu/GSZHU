import os

def cmd_sys(args):
    os.system(' '.join(args))
    
def cmd_help(args):
    print('Usage: amake [-h] [-config {debug|release}]')
    print('             [-iset {intel64|arm64}]')
    print('             [-target $TARGET_NAME$]')
    print('             [-gen] [-build] [-ide] [-clean] [-clean-build] [-test]')
    print('             {win, macos, ios, android}')
    print('')