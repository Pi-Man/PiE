#include <PiE.h>

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