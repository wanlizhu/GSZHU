# Directory Structure

Overview of the directories that make up the engine and user projects.

At the top level, there is the Engine directory as well as any user projects you have. The Engine directory contains the engine itself and all of the tools that come with it. Each user project folder contains all of the files pertaining to that project. There is a much greater separation between the engine and user projects in ZHUENGINE than in previous versions of the engine.

## Root Directory

* **Engine** - Contains all source code, content, etc. that makes up the engine.
* **Samples** - Collection of project sources available
* **ZHUENGINE.sln** - Visual Studio solution file

## Common Directories

Some subdirectories are common amongst both the Engine and user project directories:

* **Binaries** - Contains executable files or other files created during compiling.
* **Build** - Holds files needed for building the engine or game, including files necessary for creating platform-specific builds.
* **Config** - Configuration files for setting values that control engine behavior. Values set in the user project Config files override the values set in the Engine\Config directory.
* **Content** -  Holds content for the engine or user project, including asset packages and maps.
* **DerivedDataCache** - Contains derived data files generated on-load for referenced content. Not having cache files present for referenced content can increase load times dramatically.
* **Intermediate** - Contains temporary files generated during building the engine or game. In user project directories, Shaders are stored in the Intermediate directory.
* **Saved** - Contains autosaves, configuration (.ini) files, and log files. Additionally, the Engine > Saved directory contains crash logs, hardware information, and options and data.
* **Source** - Contains all of the source files for engine or user project, including the engine source code, tools, gameplay classes, etc.
  * **Engine** - Source files in the Engine directory are categorized into the following:
    * **Developer** - Files used by both the editor and engine.
    * **Editor** - Files used by just the editor.
    * **Programs** - External tools used by the engine or editor.
    * **Runtime** - Files used by just the engine.
