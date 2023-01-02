## Notice

Abandoned for now but I might return to working on this project if I find some inspiration, at the moment I'm just learning modelling and more in Blender 🤷

## Premise

The original idea for this project was to create a library that could be used to easily create any small 2D or 3D games without having to use Unreal or Unity.

Basic functionality that I was plannning on implementing was importing meshes, using GLSL/HLSL shaders, default keyboard and xinput support, and audio.

## REQUIRED LIBRARIES FOR COMPILATION

- lopengl32: should exist on your system by default.
- log.c: INCLUDED IN PROJECT, RENAMED TO GLlog.c and GLlog.h (https://github.com/rxi/log.c) (MIT LICENSE)

- lglfw3 (libglfw3.a): look up compiling glfw using cmake

- lcglm: C GLM Math library (MIT LICENSE)

    goto https://github.com/recp/cglm.
    install include folder to "include path". (UNIX: usr/include), (Windows: wherever mingw32/TDM-GCC "/include" folder is located)
    change CMakeLists.txt to "static library" at the top (THIS IS BEING DONE BECAUSE I WAS HAVING MORE TROUBLE WITH DYNAMIC LINKED LIBRARIES)
    make "build" directory in CGLM folder
    run these commands:

        cd build
        cmake ..
        make

    this should create a static library file "libcglm.a" in "/build"
    put static library file in "library path" of system, or configure makefile to check "/bin" for static library

- lglad (libglad.a): 
    
    goto https://glad.dav1d.de/ and generate loader with these specs
    
        Language = C/C++
        Specification = OpenGL
        API
            gl = 4.6
        Profile = Core

    open zip:

    Paste both folders in "include" into your compiler's "include path" destination. (UNIX: usr/include), (Windows: wherever mingw32/TDM-GCC "/include" folder is located)
    
    Move "glad.c" to directory out of zip.
    
    Execute the command `clang -c glad.c` (it does not have to be clang, it can be any compiler). This should make a `glad.o` file in the same directory.
    
    Within the same directory, execute the command `ar rcs libglad.a glad.o` to create a statically linked library `libglad.a` within the same directory.
    
    Copy `libglad.a` into your compiler's "library path" destination. (UNIX: usr/lib), (Windows: wherever mingw32/TDM-GCC "/lib" folder is located)


- Libraries can also be linked using the "-L/libpath" flag to point to a folder that contains the libraries
