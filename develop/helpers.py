import os
import sys
import argparse
import datetime
import shutil
import subprocess as subproc


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--boost', action='store_true', help='link boost library')
    parser.add_argument('--target-platform', default='', help='target platform to build for')

    parser.add_argument('--clear', action='store_true', help='clear cmake generated files')
    parser.add_argument('--ide', action='store_true', help='generate targets and open them in IDE')
    parser.add_argument('--test', '-t', action='store_true', help='run test cases')
    parser.add_argument('--buildall', action='store_true', help='build all targets')
   
    args = parser.parse_args()
    if args.target_platform is '':
        if sys.platform.startswith('win'):
            args.target_platform = 'windows'
        elif sys.platform.startswith('darwin'):
            args.target_platform = 'macos'
        elif sys.platform.startswith('linux'):
            args.target_platform = 'linux'
        else: assert(False)
    return args


def run_command(cmd, logfile=os.devnull, show_stdout=True, env=None, cwd=None):
    if logfile is not os.devnull:
        os.makedirs(os.path.dirname(os.path.abspath(logfile)), exist_ok=True)

    log = open(logfile, 'a')
    output = ''
    proc = subproc.Popen(cmd, stderr=subproc.STDOUT, stdout=subproc.PIPE,
                         shell=True, env=env, cwd=cwd)
    while True:
        line = proc.stdout.readline().decode('utf-8')
        if line == '' and proc.poll() is not None:
            break
        if line: 
            output += line
            if show_stdout: print(line, end='', flush=True)
            line = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + '    ' + line
            log.write(line.strip() + '\n')

    log.close()
    rc = proc.returncode
    if rc is not 0:
        sys.stderr.write('Error in running command.\n\n')
        raise Exception('Command: "%s"\nExit Code: %d' % (cmd, rc))
    
    return output


def check_vcpkg():
    if os.environ.get('VCPKG_PATH') is None:
        try:
            is_windows = sys.platform.startswith('win')
            bin_dir = 'D:\\' if is_windows else '/usr/local/bin'
            vcpkg_path = os.path.join(bin_dir, 'vcpkg')
            bootstrap = 'bootstrap-vcpkg.bat' if is_windows else 'bootstrap-vcpkg.sh'

            if not os.path.exists(os.path.join(vcpkg_path, 'vcpkg.exe' if is_windows else 'vcpkg')):
                run_command('git clone --depth=1 https://github.com/Microsoft/vcpkg.git', cwd=bin_dir)
                run_command(bootstrap, cwd=vcpkg_path)
                run_command('vcpkg integrate install', cwd=vcpkg_path)

            os.environ['VCPKG_PATH'] = vcpkg_path
            print('Downloading VCPKG: DONE.', flush=True)
        except Exception as err:
            print('Downloading VCPKG: FAILED.\n%s' % (str(err)), end='\n\n', flush=True)
            os.system('pause')
            return
    
    print('VCPKG: ', os.environ['VCPKG_PATH'], flush=True)