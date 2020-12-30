# PiE
A game engine I started working on.
Written in C++

# Supports
GameObjects
Components
RenderObjects
RenderContexts per RenderObject
Transforms supporting parent Transforms
Custom render functions
Wavefront OBJ loader
Fixed Update loop and separate Render loop with the option of having the render loop on a separate thread
Low Level Audio (will be upgraded to OpenAL)
Point Lights and Directional Lights
up to 4 point lights and 4 directional lights per render object
2D Perlin Noise generator
Height Map mesh generation
# Using in VS
copy all folders but "DLLs" to the VS project folder
copy all the dlls from the "DLLs" folder into the VS project folder
optionally copy any assets you would like to use into the VS project folder or sub folders
add the include folders from each of the dependency folders to the include directories setting
add the dependency folders to the library directories setting
add the .lib files to the linker input additional dependencies setting

    PiE.lib (PiEd.lib for debug configuration)
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib
    glew32.lib
    opengl32.lib
    portaudio_x64.lib
    
set the linker subsystem to Console (this is required by SDL)

note: if you still are unable to build a project, make sure your build settings are correct and that you set the settings for the correct build configuration

# Building in VS
included is a VS project in "source" that you should be able to just open wtih VS
