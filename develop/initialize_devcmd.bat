@echo off

set VsDevCmd1="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"

set VsDevCmd2="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"

set VsDevCmd3="D:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"

set VsDevCmd4="D:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"

if exist %VsDevCmd1% call %VsDevCmd1%
if exist %VsDevCmd2% call %VsDevCmd2%
if exist %VsDevCmd3% call %VsDevCmd3%
if exist %VsDevCmd4% call %VsDevCmd4%



