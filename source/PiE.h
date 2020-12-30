#ifndef _PIE
#define _PIE

#include <iostream>
#include <Windows.h>
#include <map>
#include <thread>

#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>

#include "Macros.h"
#include "FPSCounter.h"
#include "Shader.h"
#include "VertexArrayObject.h"
#include "Camera5DoF.h"
#include "UpdateLimiter.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "PerlinNoise2D.h"
#include "GL_Texture.h"
#include "Microphone.h"
#include "Speaker.h"
#include "obj.h"
#include "Utils.h"
#include "ObjComponent.h"
#include "DirectionalLight.h"
#include "PointLight.h"

namespace PiE {

	typedef SDL_Point Vec2i;

	struct EngineContext;

	// intialize the engine
	// * if initialization fails, returns a non-zero number
	int initEngine(EngineContext &ctx);

	// stops all loops and closes all subsytems
	int shutdownEngine(EngineContext& ctx);

	// add a render object to the context
	int addMesh(EngineContext &ctx, RenderObject &vertexArrayObject);

	// add a game object to the context
	int addGameObject(EngineContext &ctx, GameObject &gameObject);

	// get the current time fraction between fixed updates
	double getRenderPartialTick(const EngineContext &ctx);

	// set the render context as the current render context
	void setRenderContext(EngineContext &ctx, const RenderContext &renderContext);

	// default render pipeline
	void render(EngineContext &ctx, Camera *camera, GLuint shaderID);

	// start the render loop on a separate thread from the fixed update loop
	std::thread startRenderThread(EngineContext &ctx);

	// called by startRenderThread(EngineContext &ctx) to start the render loop
	void startRenderLoop(EngineContext &ctx);

	// starts the main fixed update loop
	// * render: specifies if rendering is done in this loop or not
	// ** if rendering is in it's own thread (or you do not wish to have a render loop) set this to false
	void startMainGameLoop(EngineContext &ctx, const bool render);

	// holds all of the data used by the game engine
	struct EngineContext {

		std::mutex loopMutex;

		// a referenc to the game window
		SDL_Window* mainWindow;

		// the GL context object
		SDL_GLContext glContext;

		// the currently set render context 
		RenderContext lastRenderContext;

		// the update limiters for the fixed update loop and the render loop
		UpdateLimiter tickLimiter, renderLimiter;

		// minimum required OpenGL major version number
		const int OPENGL_MAJOR_VERSION;
		// minimum required OpenGL minor version number
		const int OPENGL_MINOR_VERSION;

		// MSAA level
		const int MSAA;

		// the size to set the game window to
		Vec2i windowSize;

		// the camera to use to render
		Camera *mainCamera;
		// the default shader to use to render
		GLuint mainShader;

		// the list of render objects to render
		std::vector<RenderObject*> renderObjects;
		// the list of transforms to update previous states
		std::vector<Transform*> gameObjectTransforms;
		// the list of all the directionla lights
		std::vector<DirectionalLight*> dirLights;
		// the list of all the point lights
		std::vector<PointLight*> pointLights;

		// a list of functions to be executed once every fixed update
		std::vector<std::function<void(EngineContext&)>> fixedUpdate{ 
			[](EngineContext &ctx) {
				int flags = SDL_GetWindowFlags(ctx.mainWindow);
				if (!(flags & SDL_WINDOW_INPUT_FOCUS)) {
					if (SDL_GetRelativeMouseMode()) SDL_SetRelativeMouseMode(SDL_FALSE);
				}
			}
		};

		// a list of functions to be executed at the specified event type
		std::multimap<Uint32, std::function<void(EngineContext&, SDL_Event)>> events = { 
			{SDL_EventType::SDL_QUIT,
				[](EngineContext &ctx, SDL_Event event) {shutdownEngine(ctx); }}
		};

		// the function to be called to render on every render loop
		void(*renderer)(EngineContext &ctx, Camera *camera, GLuint shaderID) = render;
		//std::function<void(EngineContext &ctx, Camera *camera, GLuint shaderID)> renderer = render;

		// whether the update loop and the render loops are running
		// * should not be modified direcly, use Engine::shutdownEngine(EngineContext& ctx) instead
		bool running = false;

		// whether or not render interpolation should be active or not
		bool lerp = false;
		
		EngineContext(unsigned int fps = 60, unsigned int tps = 30, const int OPENGL_MAJOR_VERSION = 3, const int OPENGL_MINOR_VERSION = 0, const int MSAA = -1, Vec2i windowSize = { 600, 600 }) :
			tickLimiter(tps, false),
			renderLimiter(fps, false),
			OPENGL_MAJOR_VERSION(OPENGL_MAJOR_VERSION),
			OPENGL_MINOR_VERSION(OPENGL_MINOR_VERSION),
			MSAA(MSAA),
			windowSize(windowSize)
			{}

	};
	
}

#endif