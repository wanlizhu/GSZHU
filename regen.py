import os
import sys
import datetime
import shutil
import subprocess as subproc
from utils import *

check_vcpkg()

ARGS = parse_args()
VCPKG_PATH = os.environ['VCPKG_PATH']
TOOLCHAIN_FILE = '-DCMAKE_TOOLCHAIN_FILE="%s"' % os.path.realpath('%s/scripts/buildsystems/vcpkg.cmake' % (VCPKG_PATH))
LOG_FILE = os.path.realpath('%s/build/cmake.log' % (os.getcwd()))
GEN_PATH = 'build'
SRC_AND_DST = '-S . -B "%s"' % (GEN_PATH)
DEPENDENCIES = ['glog', 'gtest', 'glfw3', 'eigen3', 'tinyxml2', 'nlohmann-json', 'pybind11',
                'cgal' if ARGS.boost else '', 'boost' if ARGS.boost else '']
if sys.platform.startswith('win'):
    GENERATOR = '-G "Visual Studio 16 2019" -A x64'
    DEPENDENCIES = [item + ':x64-windows' for item in DEPENDENCIES if item is not '']
elif sys.platform.startswith('darwin'):
    GENERATOR = '-G Xcode'
    DEPENDENCIES = [item + ':x64-osx' for item in DEPENDENCIES if item is not '']
elif sys.platform.startswith('linux'):
    GENERATOR = '-G "Unix Makefiles"'
    DEPENDENCIES = [item + ':x64-linux' for item in DEPENDENCIES if item is not '']
else: assert(False)




# remove 'build' directory
def clear():
    if os.path.exists('build'):
        try:
            shutil.rmtree('build')
        except shutil.Error as err:
            os.system('pause')

def check_dependencies():
    # vcpkg executable
    vcpkg_exe = os.path.join(VCPKG_PATH, 'vcpkg')

    # list installed packages
    installed = run_command('%s list' % (vcpkg_exe), LOG_FILE, show_stdout=False).splitlines()
    names = [item.split()[0] for item in installed]

    # install missing packages
    for dependency in ( item for item in DEPENDENCIES if item not in names):
        try:
            cmd = '%s install %s' % (vcpkg_exe, dependency)
            run_command(cmd, LOG_FILE)
        except Exception as err:
            print('Check dependencies: FAILED.', end='\n\n', flush=True)
            os.system('pause')
            return

    for info in (info for info in installed 
                      for dep in DEPENDENCIES 
                      if info.startswith(dep)):
        print(info, end='\n', flush=True)
    print('Check dependencies: DONE.', end='\n\n', flush=True)




if __name__ == '__main__':
    if ARGS.clear:
        clear()
    check_dependencies()

    # cmake generate
    os.makedirs('build', exist_ok=True)
    try:
        defines = ' '.join(['-Wno-dev', 
                            '-DZHU_USE_BOOST=1' if ARGS.android else '',
                            '-DZHU_TARGET_PLATFORM_ANDROID=1' if ARGS.android else ''])
        cmd = ' '.join(['cmake', SRC_AND_DST, GENERATOR, TOOLCHAIN_FILE, defines])
        run_command(cmd, LOG_FILE)
        
        if ARGS.ide:
            cmd = ' '.join(['cmake', '--open', GEN_PATH])
            run_command(cmd)
    except Exception as err:
        os.system('pause')    
