#include <PiE.h>
#include <Callbacks.h>

Camera5DoF *camera;
float zoom = 1;

Uniform z;
Uniform colorShift;
bool julia = false;

GameObject juliaObj;
struct JuliaComp : Component {
	int tick = 0;

	virtual void fixedUpdate(PiE::EngineContext &ctx) {
		colorShift.data.f[0] = tick++ / 300.0f;
	}

	virtual void onAdded(PiE::EngineContext &ctx, GameObject &gameObject) {
		tick = 0;
	}
};

void cameraControlls(PiE::EngineContext &ctx, SDL_Event event) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
		camera->move(0.0f, zoom / 10.0f, 0.0f);
		break;
	case SDLK_s:
		camera->move(0.0f, -zoom / 10.0f, 0.0f);
		break;
	case SDLK_d:
		camera->move(zoom / 10.0f, 0.0f, 0.0f);
		break;
	case SDLK_a:
		camera->move(-zoom / 10.0f, 0.0f, 0.0f);
		break;
	case SDLK_SPACE:
		if (julia) {
			PiE::removeGameObject(ctx, juliaObj);
			julia = false;
		}
		else {
			PiE::addGameObject(ctx, juliaObj);
			julia = true;
		}
		break;
	}
}

void uniformControlls(PiE::EngineContext &ctx, SDL_Event event) {
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		z.data.f[1] += 0.001f;
		break;
	case SDLK_DOWN:
		z.data.f[1] -= 0.001f;
		break;
	case SDLK_RIGHT:
		z.data.f[0] += 0.001f;
		break;
	case SDLK_LEFT:
		z.data.f[0] -= 0.001f;
		break;
	}
}

void zoomCamera(PiE::EngineContext &ctx, SDL_Event event) {
	zoom *= pow(1.125f, event.wheel.y);
	camera->FOV = 1/zoom;
}

int main(int argc, char** args) {
	//-------create context and initialize engine---------
	PiE::EngineContext ctx;

	PiE::initEngine(ctx);

	SDL_SetWindowTitle(ctx.mainWindow, "Mandelbrot Set");
	//----------set up main camera and shader-------------
	camera = new Camera5DoF(ctx.mainWindow);

	ctx.mainCamera = camera;

	Shader shader;

	shader.buildShader("vertexshader.txt", "fragmentShader.txt");
	z.init("z0", shader.ID, VEC2); // initialize uniform
	z.data.f[0] = z.data.f[1] = 0.0f; // and set the data to be zero initially
	colorShift.init("colorShift", shader.ID, F);
	shader.uniforms.push_back(&z); // add the uniform to the shader
	shader.uniforms.push_back(&colorShift);

	ctx.mainShader = &shader;
	//---------------add in game objects------------------
	JuliaComp j;
	juliaObj.components.push_back(&j);
	//------------add in renderable objects---------------
	RenderObject RO;
	RO.VAO = VertexArrayObject();
	RO.VAO.addQuadWithNormals({ // simple rect from 4,4 to -4,-4 to display mandelbrot results
		-4, -4, 1.0,
			1.0, 0.0, 0.0,
		4, -4, 1.0,
			0.0, 1.0, 0.0,
		4, 4, 1.0,
			0.0, 0.0, 1.0,
		-4, 4, 1.0,
			1.0, 1.0, 1.0 });
	
	PiE::addMesh(ctx, RO);
	//--------------add in event listeners----------------
	ctx.events.insert({ SDL_EventType::SDL_KEYDOWN, PiE::EventCallback(cameraControlls) });
	ctx.events.insert({ SDL_EventType::SDL_KEYDOWN, PiE::EventCallback(uniformControlls) });
	ctx.events.insert({ SDL_EventType::SDL_MOUSEWHEEL, PiE::EventCallback(zoomCamera) });
	//-------------------run main loop--------------------
	camera->move(0, 0, -1);
	PiE::startMainGameLoop(ctx, true);

	return 0;
}