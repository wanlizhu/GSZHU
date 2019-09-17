
# Current Development Status

* Implement **plugins/FabricGML**
    - Computational Geometry Math Library.
    - Make boost an optional dependency of CGAL by excluding codes using boost from compiling.

* Implement **plugins/FabricRHI**
    - Rendering Hardware Interface.
    - OpenGL is the default rendering backend, Vulkan and D3D12 is optional (can be loaded dynamically).
    - Platform specified window creation can be handled by user application or leave it to this engine as well.

* Implement **Python GUI layer**
    - Use Python(tkinter) as the GUI layer, no need to add a dependency to Qt or equivalent.
    - This engine will also export APIs for Python, **(Python call C++)**
    - **(Abeyant)** Use Lua as the extension of C++,  **(C++ call Lua)**

---


# Jottings

* **I wouldn't advise distributing a static library, however, since it's generally tied to a specific version of the compiler**




* **If I am supplying a library with libs and headers how do I supply debug symbols to a user of my library**

**Method 1: The Program Database (.pdb) Way (/Zi or /ZI)**

1. Static Lib Project: Generate a pdb with same name as your static lib:

    - Open Solution Explorer from the View menu.
    - Right click your static lib project, select Properties
    - Edit Configuration Properties->C/C++->General->Debug Information to /Zi or /ZI
        - Note that /ZI allows "Edit and Continue" editing during debugging
    - Edit Configuration Properties->C/C++->Output Files->Program Database File Name to $(OutDir)$(TargetName).pdb
    - Now compile it, and note where YourLib.lib and YourLib.pdb are.

2. Application Project: Link your executable with the static lib, and new PDB file:

    - Again, navigate to project properties, but this time, for your Application project
    - Again, edit Debug Information property as needed.
    - Edit Configuration Properties->Linker->General->Additional Library Directories, adding your own "libs" directory, or whatever directory you plan to keep/copy your YourLib.lib and YourLib.pdb files.
    - Edit Configuration Properties->Linker->Input->Additional Dependencies, adding YourLib.lib (no path in front)
    - Now copy **both YourLib.lib and YourLib.pdb** to the directory you specified above.

**Method 2: The Embedded Symbols (no .pdb) Way (/Z7)**

1. Static Lib Project: Generate a static lib with embedded debug symbols

    - As in Method 1, navigate to project properties
    - As in Method 1, modify your Debug Information, **but this time to/Z7**
    - As in Method 1, compile and note where YourLib.lib is generated.

2. Application Project: Link you executable with the static lib

    - As in Method 1, navigate to project properties
    - As in Method 1, modify your Debug Information property as needed
    - As in Method 1, edit Additional Library Directories
    - As in Method 1, edit Additional Dependencies
    - Now copy YourLib.lib to the directory specified in Additional Library Directories
    
**Discussion:**

    * **Advantages of Z7**? It's simpler, and the "Single-file" way of doing it. All the debug info is in the lib file.

    * **Disadvantages of Z7**? File size on-disk, link times, incompatible with "Minimal rebuild" (/Gm) feature, does not allow "Edit and Continue", older format (e.g. older paradigm)

    * **Why don't I specify Debug Information Setting for Application Project**? This post is concerned with how to get debug working in static lib code. The same "Method 1 vs Method 2" choice applies for the Application project as well.




* **LINK : warning LNK4098: defaultlib "LIBCMT" conflicts with use of other libs; use /NODEFAULTLIB:library**

Linker Tools Warning LNK4098
defaultlib "library" conflicts with use of other libs; use /NODEFAULTLIB:library

You are trying to link with incompatible libraries.

Important   The run-time libraries now contain directives to prevent mixing different types. You’ll receive this warning if you try to use different types or debug and non-debug versions of the run-time library in the same program. For example, if you compiled one file to use one kind of run-time library and another file to use another kind (for example, single-threaded versus multithreaded) and tried to link them, you’ll get this warning. You should compile all source files to use the same run-time library. See the Use Run-Time Library (MD, /ML, /MT, /LD) compiler options for more information. 

You can use the linker’s /VERBOSE:LIB switch to determine which libraries the linker is searching. If you receive LNK4098 and want to create an executable file that uses, for example, the single-threaded, non-debug run-time libraries, use the /VERBOSE:LIB option to find out which libraries the linker is searching. The linker should print LIBC.LIB and not LIBCMT.LIB, MSVCRT.LIB, LIBCD.LIB, LIBCMTD.LIB, or MSVCRTD.LIB as the libraries searched. You can tell the linker to ignore the the incorrect run-time libraries by typing the incorrect libraries in the Ignore Libraries text box on the Link tab of the Settings dialog box in Developer’s Studio or by using the /NODEFAULTLIB:library option with LINK for each library you want to ignore. See the Ignore Libraries (/NODEFAULTLIB) linker option for more information.

The table below shows which libraries should be ignored depending on which run-time library you want to use.

To use this run-time library Ignore these libraries 
Single-threaded (libc.lib) libcmt.lib, msvcrt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib 
Multithreaded (libcmt.lib) libc.lib, msvcrt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib 
Multithreaded using DLL (msvcrt.lib) libc.lib, libcmt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib 
Debug Single-threaded (libcd.lib) libc.lib, libcmt.lib, msvcrt.lib, libcmtd.lib, msvcrtd.lib 
Debug Multithreaded (libcmtd.lib) libc.lib, libcmt.lib, msvcrt.lib, libcd.lib, msvcrtd.lib 
Debug Multithreaded using DLL (msvcrtd.lib) libc.lib, libcmt.lib, msvcrt.lib, libcd.lib, libcmtd.lib 


For example, if you received this warning and you want to create an executable file that uses the non-debug, single-threaded version of the run-time libraries, you could use the following options with the linker:

/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcd.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib




* **Don't link release application with debug static library or vice versa**
    Because this may cuase errors like: _iterator_debug_level value '0' doesn't match value '2'
