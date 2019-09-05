@echo off

cmake -G "Visual Studio 16 2019" -A Win64 -S . -B build
cmake --open build
