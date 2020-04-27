import os
import shutil
import platform

PACKAGE_DIR = os.path.expanduser('~/vcpkg').replace('\\', '/')
PACKAGE_EXE = '{}/{}'.format(PACKAGE_DIR, 'vcpkg.exe' if platform.system() == 'Windows' else 'vcpkg')
PACKAGE_CONFIG = ':x64-windows' if platform.system() == 'Windows' else ':x64-osx'
PACKAGE_LIST = [
    #'opengl',
    #'glew',
    'glfw3',
    #'glslang',
    #'spirv-cross',
    #'imgui',
    'stb',
    'eigen3',
    #'assimp',
    'gtest',
    'jsoncpp',
]

errorlevel = 0
        
def preconfig():
    global errorlevel
    # The target directory doesn't exist or is empty
    if not os.path.isdir(PACKAGE_DIR) or not os.listdir(PACKAGE_DIR):
        os.system('git clone https://github.com/Microsoft/vcpkg "{}"'.format(PACKAGE_DIR))
        if platform.system() == 'Windows':
            os.system('"{}/bootstrap-vcpkg.bat"'.format(PACKAGE_DIR))
            # Add into system environment variable permanently
            # ERROR: Access to the registry path is denied.
            # os.system('setx /m PATH "{}"'.format(PACKAGE_DIR + ';' + os.environ['PATH']))
        else:
            os.system('"{}/bootstrap-vcpkg.sh"'.format(PACKAGE_DIR))
            os.system('chmod +x "{}"'.format(PACKAGE_EXE))
    
    # The target executable file doesn't exist
    if not os.path.isfile(PACKAGE_EXE):
        print('Failed to find vcpkg command {}'.format(PACKAGE_EXE))
        errorlevel = 1
        os.system("pause")
        return False
    else:
        os.system('"{}" integrate install'.format(PACKAGE_EXE))
        os.system('"{}" version'.format(PACKAGE_EXE))
        return True
        
def find_cached_package(package):
    installedPackages = os.popen('"{}" list'.format(PACKAGE_EXE)).read()

    for packageInfo in installedPackages.split('\n'):
        parts = [i for i in packageInfo.split(' ') if i != ""]
        if parts and parts[0] == package:
            return True

    return False

def install_package(package):
    global errorlevel
    if find_cached_package(package):
        print('  {}\t\t\t[OK]'.format(package))
    else:
        os.system('"{}" install {}'.format(PACKAGE_EXE, package))
        print('  {}\t\t\t[{}]'.format(package, 'OK' if find_cached_package(package) else 'Failed'))
        if not find_cached_package(package):
            errorlevel = 1
          

if __name__=='__main__':
    print('Package directory: "{}"'.format(PACKAGE_DIR))
    print('\n')

    preconfig()
    
    for packageName in PACKAGE_LIST:
        install_package(packageName + PACKAGE_CONFIG)
    
    if errorlevel > 0:
        print('errorlevel: {}'.format(errorlevel))
        os.system('pause')
        
    exit(errorlevel)