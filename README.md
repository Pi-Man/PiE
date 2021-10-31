# PiE
A simple lightweight Game Engine built on SDL2 GLEW and (soon) OpenAL (currently using Port Audio instead)

Written in C++

# Supports
GameObjects

Components

RenderObjects

RenderContexts per RenderObject

Transforms supporting parent Transforms

Custom render functions

Wavefront OBJ loader

JSON parser/printer

Fixed Update loop and separate Render loop with the option of having the render loop on a separate thread

Low Level Audio (will be upgraded to OpenAL)

Custom Shaders

Shader Uniforms (int, float, double, vec2-4, vec2-4i, vec2-4d, mat4, mat4d)

Point Lights and Directional Lights

up to 4 point lights and 4 directional lights per render object (no limit for scene)

2D Perlin Noise generator

Height Map mesh generation

# Hello Window

```cpp
int main(int argc, char** args) {
	//-------create context and initialize engine---------
	PiE::EngineContext ctx;

	PiE::initEngine(ctx);

	SDL_SetWindowTitle(ctx.mainWindow, "Hello Square");
	//----------set up main camera and shader-------------
	Camera5DoF camera{ ctx.mainWindow };

	ctx.mainCamera = &camera;

	Shader shader;

	shader.buildShader("vertexshader.txt", "colorFragmentShader.txt");

	ctx.mainShader = &shader;
	//------------add in render objects---------------
	RenderObject RO;
	RO.VAO = VertexArrayObject();
	RO.VAO.addQuad({ // simple rect from 1,1 to -1,-1
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
	});

	PiE::addMesh(ctx, RO);
	//-------------------run main loop--------------------
	camera.move(0, 0, -1);
	PiE::startMainGameLoop(ctx, true);

	return 0;
}
```

this will render a single quad the width of the window

# Using in VS
copy all folders but the "DLLs" folder to the VS project folder

copy all the dlls from the "DLLs" folder into the VS project folder

optionally copy any assets you would like to use into the VS project folder or sub folders

add the include folders from each of the dependency folders to the include directories setting.  **MAKE SURE YOU HAVE THE CONFIGURATION(S) YOU WANT TO USE SELECTED**

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

# Using VS Template Project

swap branch to Template Project and clone

find the .sln file and open

# Building in VS

open PiE.sln with VS

right click the PiE project (as opposed to one of the example projects) and click build or rebuild

look for the .lib in the x64 folder
