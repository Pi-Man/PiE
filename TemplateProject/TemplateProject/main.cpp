#include <PiE.h>

Camera5DoF *camera;
float zoom = 1;

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

	ctx.mainShader = shader.ID;
	//------------add in renderable objects---------------
	RenderObject RO;
	RO.VAO = VertexArrayObject();
	RO.VAO.addQuadWithNormals({ // simple rect from 2,2 to -2,-2 to display mandelbrot results
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
	ctx.events.insert({ SDL_EventType::SDL_KEYDOWN, cameraControlls });
	ctx.events.insert({ SDL_EventType::SDL_MOUSEWHEEL, zoomCamera });
	//-------------------run main loop--------------------
	camera->move(0, 0, -1);
	PiE::startMainGameLoop(ctx, true);

	return 0;
}