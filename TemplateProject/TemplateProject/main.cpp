#include <PiE.h>

int main(int argc, char** args) {
	//-------create context and initialize engine---------
	PiE::EngineContext ctx;

	PiE::initEngine(ctx);

	SDL_SetWindowTitle(ctx.mainWindow, "Hello Square");
	//----------set up main camera and shader-------------
	Camera5DoF camera(ctx.mainWindow);

	ctx.mainCamera = &camera;

	Shader shader;

	shader.buildShader("vertexshader.txt", "fragmentShader.txt");

	ctx.mainShader = shader.ID;
	//------------add in renderable objects---------------
	RenderObject RO;
	RO.VAO = VertexArrayObject(VAO_NORMALS); // using normals for color because color isn't implemented yet
	RO.VAO.addQuadWithNormals({
		-0.5, -0.5, 1.0,
			1.0, 0.0, 0.0,
		0.5, -0.5, 1.0,
			0.0, 1.0, 0.0,
		0.5, 0.5, 1.0,
			0.0, 0.0, 1.0,
		-0.5, 0.5, 1.0,
			1.0, 1.0, 1.0 });
	
	PiE::addMesh(ctx, RO);
	//-------------------run main loop--------------------
	camera.move(0, 0, -1);
	PiE::startMainGameLoop(ctx, true);

	return 0;
}