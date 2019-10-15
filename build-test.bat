@echo off

call develop\initialize_devcmd.bat

if not exist build\GraphicsEngine.sln (
    call generate-x64-windows.bat
)

msbuild build\GraphicsEngine.sln /t:UnitTestGE2:Rebuild
if not %errorlevel%==0 pause
