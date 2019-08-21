@echo off

if exist "bin\" rd /s/q "bin\"
if exist "lib\" rd /s/q "lib\"
if exist "Engine\__cache__\"   rd /s/q "Engine\__cache__\"
if exist "Engine\external\GLFW\build\" rd /s/q "Engine\external\GLFW\build\"
if exist "Launcher\__cache__\" rd /s/q "Launcher\__cache__\"
if exist "UnitTest\__cache__\" rd /s/q "UnitTest\__cache__\"

