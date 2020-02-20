@ECHO OFF

REM Clean previous generated project
IF EXIST build RD/S/Q build
MD build

REM Android Studio path
SET AndroidStudio="D:\Program Files\Android\Android Studio\bin\studio64.exe"

REM Generate for VS2019 Win64
SET Compiler="Visual Studio 16 2019" -A x64
SET BuildDir=build
SET SourceDir=.
SET CMakeDefines=-DWANLI_ENABLE_VULKAN=ON ^
-DWANLI_ENABLE_D3D12=OFF ^
-DWANLI_BUILD_ANDROID=ON

cmake -B %BuildDir% -S %SourceDir% -G %Compiler% %CMakeDefines%

REM Open generated project if successful
IF ERRORLEVEL 1 GOTO OnError ELSE (
	cmake --open %BuildDir%
	%AndroidStudio% "editor\Android\WanliEditor"
)

GOTO EOF

:OnError
ECHO Failed to generate project.
PAUSE
