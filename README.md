# Prerequisites

* Install CMake and added to PATH
* Install Python 3.x and added to PATH
* Window10 SDK is required on Windows
* (optional) Qt5 is required to build Editor

# How to build?

* run "**startup.py**" script from current directory, this process will check and download requisite third-party libraries if necessary
* input command "**gen**", to generate project file, with following options
  * "**-no editor**": exclude editor target from generation
  * "**-no test**": exclude test target from generation
  * "**-no benchmark**": exclude benchmark target from generation
* input command "**open**", to launch platform specified IDE
