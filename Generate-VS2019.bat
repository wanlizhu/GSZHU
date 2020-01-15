@ECHO OFF

REM Clean previous generated project
IF EXIST build RD/S/Q build
MD build

REM Generate for VS2019 Win64
SET Compiler="Visual Studio 16 2019" -A x64
SET BuildDir=build
SET SourceDir=.
SET CMakeDefines=-DWANLIX_ENABLE_VULKAN=ON ^
-DWANLIX_ENABLE_VULKAN=OFF ^
-DWANLIX_BUILD_EDITOR=ON

cmake -B %BuildDir% -S %SourceDir% -G %Compiler% %CMakeDefines%

REM Open generated project if successful
IF ERRORLEVEL 1 GOTO OnError ELSE (
	cmake --open %BuildDir%
)

GOTO EOF

:OnError
ECHO Failed to generate project.
PAUSE
