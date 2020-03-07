# Graphics Engine

A collection of source code for multithread rendering in Next-Gen API: D3D12 and Vulkan.

The engine is written in C++ 17 and, as such, has portable access to standard constructs for multithreading programming on cores. The engine also supports high-performance computing using general purpose GPU programming (GPGPU). SIMD code is also available using Intel Streaming SIMD Extensions (SSE).

## Designing and Readmap

* Entry Point
* Application Layer
    * Window
    * Input system (1. Post event to event system; 2. Query current status)
    * Event system
* Renderer
* Render API (abstraction)
* Debugging support (logging...; compile in debug mode; performance monitoring, benchmark)
* Scripting
* Memory system
* Entity component system (ECS)
* Physics system
* File I/O, VFS
* Asset system
* Host swap (change the asset while the game is running)