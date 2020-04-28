import os
import shutil
import platform
import subprocess as sp

PACKAGE_DIR = os.path.expanduser('~/vcpkg').replace('\\', '/')
PACKAGE_EXE = '{}/{}'.format(PACKAGE_DIR, 'vcpkg.exe' if platform.system() == 'Windows' else 'vcpkg')
PACKAGE_CONFIG = ':x64-windows' if platform.system() == 'Windows' else ':x64-osx'
PACKAGE_LIST = [
    'glfw3',
    #'glslang',
    #'imgui',
    'stb',
    'eigen3',
    #'assimp',
    'gtest',
    'jsoncpp',
]

errorlevel = 0
installed_list_cache = []
package_index = 0

def run(cmd, return_output = False):
    output = None
    
    if return_output:
        err, output = sp.getstatusoutput(cmd)
    else:
        err = os.system(cmd)
        
    if err != 0:
        print('  subprocess exit code: ', err)
    
    return output
    
def preconfig():
    global errorlevel
    # The target directory doesn't exist or is empty
    if not os.path.isdir(PACKAGE_DIR) or not os.listdir(PACKAGE_DIR):
        run('git clone https://github.com/Microsoft/vcpkg "{}"'.format(PACKAGE_DIR))
        
        if platform.system() == 'Windows':
            run('"{}/bootstrap-vcpkg.bat"'.format(PACKAGE_DIR))
        else:
            run('"{}/bootstrap-vcpkg.sh"'.format(PACKAGE_DIR))
            run('chmod +x "{}"'.format(PACKAGE_EXE))
    
    # The target executable file doesn't exist
    if not os.path.isfile(PACKAGE_EXE):
        print('Failed to find vcpkg command {}'.format(PACKAGE_EXE))
        errorlevel = 1
        return False
    else:
        run('"{}" integrate install'.format(PACKAGE_EXE))
        run('"{}" version'.format(PACKAGE_EXE))
        refresh_cached_list()
        print('')
        return True
        
def refresh_cached_list():
    global installed_list_cache
    installedPackages = run('"{}" list'.format(PACKAGE_EXE), True)
    installed_list_cache = []
    
    for packageInfo in installedPackages.split('\n'):
        parts = [i for i in packageInfo.split(' ') if i != ""]
        if parts and parts[0]:
            installed_list_cache.append(parts[0])

def install_package(package):
    global errorlevel
    global installed_list_cache
    global package_index
    
    package_index += 1
    
    if package in installed_list_cache:
        print('  {}\t\t\t{}/{} [OK]'.format(package, package_index, len(PACKAGE_LIST)))
    else:
        max_retry = 2
        retry = 0
        success = False
        
        while retry <= max_retry and not success:
            retry_info = '' if retry == 0 else ' (retry {}/{})'.format(retry, max_retry)
            print('')
            print('  installing{} missing package ({}/{}):\t{}'.format(retry_info, package_index, len(PACKAGE_LIST), package))
            print('')
            
            run('"{}" install {}'.format(PACKAGE_EXE, package))
            refresh_cached_list()
            success = package in installed_list_cache
            retry += 1
        
        print('  {}\t\t\t{}/{} [{}]'.format(package, package_index, len(PACKAGE_LIST), 'OK' if success else 'Failed'))
        errorlevel = errorlevel if success else 1
          

if __name__=='__main__':
    print('Package directory: "{}"'.format(PACKAGE_DIR))
    print('')

    if preconfig():   
        for packageName in PACKAGE_LIST:
            install_package(packageName + PACKAGE_CONFIG)
    
    if errorlevel > 0:
        print('errorlevel: {}'.format(errorlevel))
        os.system('pause')
        
    exit(errorlevel)