import os
import sys
import datetime
import shutil
import subprocess as subproc
from regenutils import *

check_vcpkg()

ARGS = parse_args()
VCPKG_PATH = os.environ['VCPKG_PATH']
TOOLCHAIN_FILE = '-DCMAKE_TOOLCHAIN_FILE="%s"' % os.path.realpath('%s/scripts/buildsystems/vcpkg.cmake' % (VCPKG_PATH))
LOG_FILE = os.path.realpath('%s/build/%s/cmake.log' % (os.getcwd(), ARGS.target_platform))
GEN_PATH = 'build/%s' % (ARGS.target_platform)
SRC_AND_DST = '-S . -B "%s"' % (GEN_PATH)
DEPENDENCIES = ['glog', 'gtest', 'glfw3', 'eigen3', 'tinyxml2', 'nlohmann-json', 'pybind11',
                'cgal' if ARGS.boost else '', 'boost' if ARGS.boost else '']
if ARGS.target_platform is 'windows': 
    GENERATOR = '-G "Visual Studio 16 2019" -A x64'
    DEPENDENCIES = [item + ':x64-windows' for item in DEPENDENCIES if item is not '']
elif ARGS.target_platform is 'macos':
    GENERATOR = '-G Xcode'
    DEPENDENCIES = [item + ':x64-osx' for item in DEPENDENCIES if item is not '']
elif ARGS.target_platform is 'linux':
    GENERATOR = '-G "Unix Makefiles"'
    DEPENDENCIES = [item + ':x64-linux' for item in DEPENDENCIES if item is not '']
elif ARGS.target_platform is 'android':
    pass
else: assert(False)




# remove 'build' directory
def clear():
    if os.path.exists(GEN_PATH):
        try:
            shutil.rmtree(GEN_PATH)
        except shutil.Error as err:
            print(str(err))
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
            print('Check dependencies: FAILED.\n%s' % (str(err)), end='\n\n', flush=True)
            os.system('pause')
            return False

    for info in (info for info in installed 
                      for dep in DEPENDENCIES 
                      if info.startswith(dep)):
        print(info, end='\n', flush=True)
    print('Check dependencies: DONE.', end='\n\n', flush=True)
    return True




if __name__ == '__main__':
    if ARGS.clear:
        clear()

    if not check_dependencies():
        exit(1)

    # cmake generate
    os.makedirs(GEN_PATH, exist_ok=True)
    try:
        defines = ' '.join(['-Wno-dev', 
                            '-DZHU_USE_BOOST=1' if ARGS.boost else ''])
        cmd = ' '.join(['cmake', SRC_AND_DST, GENERATOR, TOOLCHAIN_FILE, defines])
        run_command(cmd, LOG_FILE)
        
        if ARGS.ide:
            cmd = ' '.join(['cmake', '--open', GEN_PATH])
            run_command(cmd, LOG_FILE)

        if ARGS.buildall:
            cmd = ' '.join(['cmake', '--build', GEN_PATH])
            run_command(cmd, LOG_FILE)

        if ARGS.test:
            cmd = ' '.join(['cmake', '--build', GEN_PATH])
            run_command(cmd, LOG_FILE)
            cmd = os.path.realpath(os.path.join(GEN_PATH, bin, 'GraphicsEngine_UnitTest'))
            run_command(cmd, LOG_FILE)

    except Exception as err:
        print(str(err))
        os.system('pause')    
