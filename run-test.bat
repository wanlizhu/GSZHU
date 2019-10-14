@echo off

if not exist build\bin\UnitTestGE2.exe (
    call build-test.bat
)
start build\bin\UnitTestGE2.exe