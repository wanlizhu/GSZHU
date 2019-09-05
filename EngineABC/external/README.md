* **glfw3**

    - Build with VC142 (Visual Studio 2019), platform is x64
    - Macro "GLFW_VULKAN_STATIC" is off
    - **Debug**: The debug information format is "**/Z7**" which has all the debug information embedded in the generated .lib file. (no need for extra .pdb file)

* **glog**

    - Build with VC142 (Visual Studio 2019), platform is x64
    - **Debug**: The debug information format is "**/Z7**" which has all the debug information embedded in the generated .lib file. (no need for extra .pdb file)
    - **To Use As Static Library**:
        * Must add preprocessor definition: GOOGLE_GLOG_DLL_DECL=
        * This definition is optional: GLOG_NO_ABBREVIATED_SEVERITIES