#include "PiE.h"
#include "Model4D.h"
#include "Camera4D.h"

//Camera *camera;

Uniform iterations;

Model4D model;
RenderObject modelRenderObject;
Camera4D *camera4D;

bool pause = true;

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

void cameraMoveCallback(PiE::EngineContext &ctx) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[VIEW_SHIFT]) {
		if (keys[FORWARDS]) {
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
		camera4D->rotate4D<AXIS::X, AXIS::W>(20.0f / ctx.tickLimiter.getStaticUPS());
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		camera4D->rotate4D<AXIS::X, AXIS::W>(-20.0f / ctx.tickLimiter.getStaticUPS());
	}
	if (keys[SDL_SCANCODE_T]) {
		iterations.data.i[0]++;
	}
	if (keys[SDL_SCANCODE_G]) {
		iterations.data.i[0]--;
	}
	if (keys[PAUSE]) {
		SDL_SetRelativeMouseMode((SDL_bool)pause);
		pause = !pause;
	}
}

void cameraRotateEvent(PiE::EngineContext &ctx, SDL_Event event) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (!pause) {
		if (keys[VIEW_SHIFT]) {
			camera4D->rotate(-event.motion.xrel * 0.3f, event.motion.yrel * 0.3f);
		}
		else {
			camera4D->rotate4D<AXIS::Z, AXIS::X>(event.motion.xrel * 0.3f);
			camera4D->rotate4D<AXIS::Y, AXIS::Z>(event.motion.yrel * 0.3f);
		}
	}
}

void update4Dmodel(PiE::EngineContext &ctx) {
	VertexArrayObject VAO = camera4D->process4DModel(model);
	modelRenderObject.VAO.copy(VAO);
}

int main(int argc, char** args) {

	PiE::EngineContext ctx;

	PiE::initEngine(ctx);

	Shader mainShader, newtonFractalShader, wireframeColorShader;
	mainShader.buildShader("vertexShader.txt", "fragmentShader.txt");
	newtonFractalShader.buildShader("vertexShader.txt", "newtonFragmentShader.txt");
	wireframeColorShader.buildShader("vertexShader.txt", "colorFragmentShader.txt");// "wireframegeometryshader.txt");

	iterations.init("iterations", newtonFractalShader.ID, VEC1I);
	iterations.data.i[0] = 0;
	newtonFractalShader.uniforms.push_back(&iterations);
	
	camera4D = new Camera4D(ctx.mainWindow);
	camera4D->setClipPlanes(1.0f / 128.0f, 100.0f);

	camera4D = new Camera4D(ctx.mainWindow);
	//camera4D->set4DViewMatrix(camera4D->get4DViewMatrix().translate<3>(-2));

	ctx.mainCamera = camera4D;
	ctx.mainShader = &mainShader;

	RenderObject RO;
	PerlinNoise2D noise(1);
	std::vector<std::vector<float>> hmap;
	for (int i = 0; i < 400; i++) {
		hmap.push_back(std::vector<float>{});
		for (int j = 0; j < 400; j++) {
			hmap[i].push_back((float) pow((i-200)/100.0, (j-200)/100.0));
		}
	}
	RO.VAO = VertexArrayObject(VAO_NORMALS | VAO_UVS);
	RO.VAO.addHeightMesh(0, 0, 10, 10, hmap);
	RO.renderContext.clearColor = COLOR(255, 0, 0);

	RenderObject newtonFractal;
	newtonFractal.VAO.addQuad({
		-0.5f, 0, -0.5f,
		0.5f, 0, -0.5f,
		0.5f, 0, 0.5f,
		-0.5f, 0, 0.5f
		});
	newtonFractal.transform.m.translate({ { 5, 3, 2 } });
	newtonFractal.shader = newtonFractalShader;

	model.addCenteredHyperCuboid();
	//model.matrix.translate<AXIS::W>(3);
	modelRenderObject.shader = wireframeColorShader;
	modelRenderObject.renderContext.depthEnable = false;
	modelRenderObject.renderContext.cullEnable = true;

	float r = 18.0f / M_PI;

	camera4D->move4D(0, 0, -2.0f/r, -r);
	camera4D->FOV = 0.5f;
	//camera4D->rotate(0, 45);

	DirectionalLight light;
	ctx.dirLights.push_back(&light);

	ctx.fixedUpdate.push_back(PiE::FixedUpdateCallback(cameraMoveCallback));

	ctx.fixedUpdate.push_back(PiE::FixedUpdateCallback(update4Dmodel));

	ctx.events.insert({ SDL_MOUSEMOTION, PiE::EventCallback(cameraRotateEvent) });

	PiE::addMesh(ctx, RO);
	PiE::addMesh(ctx, newtonFractal);
	PiE::addMesh(ctx, modelRenderObject);

	PiE::startMainGameLoop(ctx, true);

	return 0;
}