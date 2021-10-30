#include <PiE.h>
#include <Model4D.h>
#include <Camera4D.h>
#include <json.h>

Model4D hyperCubeModel;
Model4D groundModel;

RenderObject hyperCube;
RenderObject ground;

Camera4D *camera4D;

bool pause = true;

// define keybinds to easily be changed if needed
enum KeyBind {
	PAUSE = SDL_SCANCODE_ESCAPE,
	FORWARDS = SDL_SCANCODE_W,
	BACKWARDS = SDL_SCANCODE_S,
	LEFT = SDL_SCANCODE_A,
	RIGHT = SDL_SCANCODE_D,
	UP = SDL_SCANCODE_SPACE,
	DOWN = SDL_SCANCODE_LCTRL,
	IN = SDL_SCANCODE_R,
	OUT = SDL_SCANCODE_F,
	VIEW_SHIFT = SDL_SCANCODE_LSHIFT,
};

// to be added to PiE::EngineContext::FixedUpdate list and called every frame
void cameraMoveCallback(PiE::EngineContext &ctx) {
	// get data on which keys are pressed, scancodes are used as an index to query if the key is down or not
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[VIEW_SHIFT]) {
		if (keys[FORWARDS]) {
			// ctx.tickLimiter.getStaticUPS() gets the requested number of updates per second, invert to get requested time per frame
			camera4D->move(0, 0, 2.0f / ctx.tickLimiter.getStaticUPS());
		}
		if (keys[BACKWARDS]) {
			camera4D->move(0, 0, -2.0f / ctx.tickLimiter.getStaticUPS());
		}
		if (keys[DOWN]) {
			camera4D->move(0, -2.0f / ctx.tickLimiter.getStaticUPS(), 0);
		}
		if (keys[UP]) {
			camera4D->move(0, 2.0f / ctx.tickLimiter.getStaticUPS(), 0);
		}
		if (keys[LEFT]) {
			camera4D->move(-2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0);
		}
		if (keys[RIGHT]) {
			camera4D->move(2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0);
		}
	}
	else {
		if (keys[FORWARDS]) {
			camera4D->move4D(0, 0, 2.0f / ctx.tickLimiter.getStaticUPS(), 0);
		}
		if (keys[BACKWARDS]) {
			camera4D->move4D(0, 0, -2.0f / ctx.tickLimiter.getStaticUPS(), 0);
		}
		if (keys[DOWN]) {
			camera4D->move4D(0, -2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0);
		}
		if (keys[UP]) {
			camera4D->move4D(0, 2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0);
		}
		if (keys[LEFT]) {
			camera4D->move4D(-2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0, 0);
		}
		if (keys[RIGHT]) {
			camera4D->move4D(2.0f / ctx.tickLimiter.getStaticUPS(), 0, 0, 0);
		}
		if (keys[IN]) {
			camera4D->move4D(0, 0, 0, 2.0f / ctx.tickLimiter.getStaticUPS());
		}
		if (keys[OUT]) {
			camera4D->move4D(0, 0, 0, -2.0f / ctx.tickLimiter.getStaticUPS());
		}
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		/* rotations (in any number of dimensions) are actually planar phenomenone, 
		   in 3D rotations are generally specified by an axis, however, this doesn't work in 4D, 
		   so it is neccessary to specify the plane of rotation instead.
		   the axis can either be specified as AXIS::<axis> or by their number, ie x = 0, y = 1 and so on,
		   this allows for n-dimensional space 
		*/
		camera4D->rotate4D<AXIS::X, AXIS::W>(20.0f / ctx.tickLimiter.getStaticUPS());
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		camera4D->rotate4D<AXIS::X, AXIS::W>(-20.0f / ctx.tickLimiter.getStaticUPS());
	}
}

// to be added to PiE::EngineContext::Events to handle mouse movement events
void cameraRotateEvent(PiE::EngineContext &ctx, SDL_Event event) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (!pause) {
		if (keys[VIEW_SHIFT]) {
			camera4D->rotate(-event.motion.xrel * 0.3f, event.motion.yrel * 0.3f);
		}
		else {
			// equivalent to rotation in the Y axis or yaw
			camera4D->rotate4D<AXIS::Z, AXIS::X>(event.motion.xrel * 0.3f);
			// equivalent to rotation in the X axis or pitch
			camera4D->rotate4D<AXIS::Y, AXIS::Z>(event.motion.yrel * 0.3f);
		}
	}
}

void keyEvents(PiE::EngineContext & ctx, SDL_Event event) {
	switch (event.key.keysym.scancode) {
	case PAUSE:
		if (!event.key.repeat) {
			SDL_SetRelativeMouseMode((SDL_bool)pause);
			pause = !pause;
		}
	}
}

void update4Dmodel(PiE::EngineContext &ctx) {
	// here the 4D model is projected into 3D space by the camera to be rendered later
	camera4D->process4DModel(hyperCube.VAO, hyperCubeModel);
	camera4D->process4DModel(ground.VAO, groundModel);
}

int main(int argc, char** args) {

	// create context with default values
	PiE::EngineContext ctx;

	// initialize system with context
	PiE::initEngine(ctx);

	Shader mainShader, wireframeColorShader;
	mainShader.buildShader("vertexShader.txt", "fragmentShader.txt");
	wireframeColorShader.buildShader("vertexShader.txt", "colorFragmentShader.txt");// "wireframegeometryshader.txt"); // uncomment to make the hypercube a wireframe

	// create the main camera
	camera4D = new Camera4D(ctx.mainWindow); 
	// this is a small scene, so the clip planes can get close to the camera
	camera4D->setClipPlanes(1.0f / 128.0f, 100.0f); 

	// set the default camera and shader
	ctx.mainCamera = camera4D;
	ctx.mainShader = &mainShader;

	// create a simple height map from perlin noise to represent (3D) ground
	PerlinNoise2D noise(1);
	std::vector<std::vector<float>> hmap(42, std::vector<float>(42, 0));
	for (int i = 0; i < 42; i++) {
		for (int j = 0; j < 42; j++) {
			hmap[i][j] = (noise.get((i - 21) / 5.0, (j - 21) / 5.0));
		}
	}
	ground.VAO = VertexArrayObject(VAO_NORMALS | VAO_UVS);
	ground.VAO.addHeightMesh(0, 0, 10, 10, hmap);
	ground.renderContext.cullEnable = false;
	// convert the VAO to a 4D model, coppies the vertices over with a W value of 0
	// when the model is then projected back to ground.VAO, the only the positions are replaced in the VAO
	groundModel.convertVAO(ground.VAO);

	hyperCubeModel.addCenteredHyperCuboid();
	// 4D models also have their own model matrix
	hyperCubeModel.matrix.translate<AXIS::W>(3);
	hyperCube.shader = wireframeColorShader;

	camera4D->move4D(0, 0, -2.0f, -2.0);

	// add a light so the ground isn't black
	DirectionalLight light; 
	ctx.dirLights.push_back(&light);

	ctx.fixedUpdate.push_back(PiE::FixedUpdateCallback(cameraMoveCallback));

	ctx.fixedUpdate.push_back(PiE::FixedUpdateCallback(update4Dmodel));

	ctx.events.insert({ SDL_MOUSEMOTION, PiE::EventCallback(cameraRotateEvent) });

	ctx.events.insert({ SDL_KEYDOWN, PiE::EventCallback(keyEvents) });

	PiE::addMesh(ctx, ground);
	PiE::addMesh(ctx, hyperCube);

	PiE::startMainGameLoop(ctx, true);

	return 0;
}