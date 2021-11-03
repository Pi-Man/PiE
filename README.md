# PiE
A simple lightweight Game Engine built on SDL2 GLEW and (soon) OpenAL (currently using Port Audio instead)

Written in C++

## Features

| Feature                      | NotPlanned | Planned | Basic Implementation | Added | Done | Notes
|------------------------------|:----------:|:-------:|:--------------------:|:-----:|:----:|:-----
|GameObjects                   | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Components                    | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|RenderObjects                 | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|RenderContexts                | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   | per RenderObject
|Transforms                    | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   | supporting parent Transforms
|Abstract Vector/Matrix types  | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   | N dimensional using any numeric type
|Custom render pipeline        | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   |
|Wavefront OBJ loader          | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   | with .mtl support
|JSON parser/printer           | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Fixed Update loop             | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Render Loop (separate thread) | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   |
|Render Loop (same thread)     | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Low Level Audio               | 🟥         | 🟩      | 🟩                   | 🟥    | 🟥   | Using Port Audio
|High Level Audio              | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   | Using OpenAL
|Custom Shaders                | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Shader Uniforms               | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   | int, float, double, vec2-4, vec2-4i, vec2-4d, mat4, mat4d
|Point Lights                  | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Directional Lights            | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   | Up to 4 point lights and 4 directional lights per render object (no limit for scene)
|2D Perlin Noise generator     | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Height Map mesh generation    | 🟥         | 🟩      | 🟩                   | 🟩    | 🟩   |
|Render Hooks                  | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|Render Interpolation          | 🟥         | 🟩      | 🟩                   | 🟩    | 🟥   |
|Custom VertexAttributes       | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|GUI components                | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|Custom Coordinate System      | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|Data Save/Load System         | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   | Can currently be done with JSON
|Keybinding System             | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|STL Model Loader              | 🟥         | 🟩      | 🟥                   | 🟥    | 🟥   |
|4D Render Support             | 🟥         | 🟩      | 🟩                   | 🟥    | 🟥   |

## Hello Window

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

## Using in VS
1)copy all folders but the "DLLs" folder to the VS project folder

2)copy all the dlls from the "DLLs" folder into the VS project folder

3)optionally copy any assets you would like to use into the VS project folder or sub folders

4)add the include folders from each of the dependency folders to the include directories setting.  **MAKE SURE YOU HAVE THE CONFIGURATION(S) YOU WANT TO USE SELECTED**

5)add the dependency folders to the library directories setting

6)add the .lib files to the linker input additional dependencies setting

    PiE.lib (PiEd.lib for debug configuration)
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib
    glew32.lib
    opengl32.lib
    portaudio_x64.lib
    
7)set the linker subsystem to Console (this is required by SDL)

note: if you still are unable to build a project, make sure your build settings are correct and that you set the settings for the correct build configuration

## Using VS Template Project

1)swap branch to Template Project and clone

2)find the .sln file and open

## Building in VS

1)open PiE.sln with VS

2)right click the PiE project (as opposed to one of the example projects) and click build or rebuild

3)look for the .lib in the x64 folder
