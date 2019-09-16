import os
import sys
import argparse
import datetime
import shutil
import subprocess as subproc


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--boost', action='store_true', help='link boost library')
    parser.add_argument('--android', '-a', action='store_true', help='build Android target')
    parser.add_argument('--config', '-c', default='debug', help='config type: debug, release')

    parser.add_argument('--target', help='specify one target')
    parser.add_argument('--clear', action='store_true', help='clear cmake generated files')
    parser.add_argument('--ide', action='store_true', help='generate targets and open them in IDE')
    parser.add_argument('--test', '-t', action='store_true', help='run test cases')
    parser.add_argument('--buildall', action='store_true', help='build all targets')
    parser.add_argument('--install', default='dist', help='install targets')
    
    return parser.parse_args()


def run_command(cmd, logfile=os.devnull, show_stdout=True, env=None, cwd=None):
    if logfile is not os.devnull:
        os.makedirs(os.path.dirname(os.path.abspath(logfile)), exist_ok=True)

    log = open(logfile, 'a')
    output = ''
    proc = subproc.Popen(cmd, stderr=subproc.STDOUT, stdout=subproc.PIPE, shell=True,
                         env=env, cwd=cwd)
    while True:
        line = proc.stdout.readline().decode('utf-8')
        if line == '' and proc.poll() is not None:
            break
        if line: 
            output += line
            if show_stdout: print(line, end='', flush=True)
            line = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + '    ' + line
            log.write(line)

    log.close()
    rc = proc.returncode
    if rc is not 0:
        sys.stderr.write('Error in running command.\n\n')
        raise Exception('Command: "%s"\nExit Code: %d' % (cmd, rc))
    
    return output


def check_vcpkg():
    if os.environ.get('VCPKG_PATH') is None:
        print('Downloading VCPKG: ', end='')
        try:
            is_windows = sys.platform.startswith('win')
            cwd = 'D:\\' if is_windows else '/usr/local/bin'
            run_command('git clone --depth=1 https://github.com/Microsoft/vcpkg.git', cwd=cmd)

            cwd = os.path.join(cwd, 'vcpkg')
            bootstrap = 'bootstrap-vcpkg.bat' if is_windows else 'bootstrap-vcpkg.sh'
            run_command(bootstrap, cwd=cwd)
            run_command('vcpkg integrate install', cwd=cwd)

            os.environ['VCPKG_PATH'] = cwd
            print('DONE.', end='\n\n', flush=True)
        except Exception as err:
            print('FAILED.', end='\n\n', flush=True)
            os.system('pause')
            return
    
    print('VCPKG: ', os.environ['VCPKG_PATH'], flush=True)