#include <vector>

#include <PiE.h>

#include "RandomPositionRotation.h"

Shader shader;
Camera *pcamera;

bool mouseLock = false;

std::vector<GameObject*> cubes; // object pool of cubes
size_t numberOfCubes = 0;

constexpr float MOVE_SPEED = 5.0f;

constexpr int ADD_CUBE = SDL_SCANCODE_RETURN;
constexpr int REMOVE_CUBE = SDL_SCANCODE_BACKSPACE;
constexpr int MOVE_FORWARD = SDL_SCANCODE_W;
constexpr int MOVE_BACKWARD = SDL_SCANCODE_S;
constexpr int MOVE_LEFT = SDL_SCANCODE_A;
constexpr int MOVE_RIGHT = SDL_SCANCODE_D;
constexpr int MOVE_UP = SDL_SCANCODE_SPACE;
constexpr int MOVE_DOWN = SDL_SCANCODE_LSHIFT;
constexpr int MOUSE_LOCK = SDL_SCANCODE_ESCAPE;

// handles adding and removing game objects on key press
// NOE: game objects can NOT be added or removed in fixed update
// the way to add or remove a game object from fixed update is to send a custom event and handle the event
void addRemoveGameObject(PiE::EngineContext &ctx, SDL_Event event) {
	switch (event.key.keysym.scancode) {
	case ADD_CUBE:
	{
		if (numberOfCubes == cubes.size()) { // if there are no more cubes in the object pool, make a new object
			GameObject *newCube = new GameObject();
			newCube->components.push_back(new ObjComponent("uv_cube.obj", shader));
			newCube->components.push_back(new RandomPositionRotation(numberOfCubes));
			cubes.push_back(newCube);
		}
		PiE::addGameObject(ctx, *cubes[numberOfCubes]);
		numberOfCubes++;
		break;
	}
	case REMOVE_CUBE:
	{
		if (numberOfCubes > 0) {
			numberOfCubes--;
			GameObject *oldCube = cubes[numberOfCubes];
			PiE::removeGameObject(ctx, *oldCube); // this does not actually delete the game object, so it can be reused later
		}
		break;
	}
	}

}

void cameraMoveCallback(PiE::EngineContext &ctx) {
	printf("%05llu\r", numberOfCubes);
	float deltaTime = 1.0f / ctx.tickLimiter.getStaticUPS();
	// get data on which keys are pressed, scancodes are used as an index to query if the key is down or not
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[MOVE_FORWARD]) {
		pcamera->move(0, 0, MOVE_SPEED * deltaTime);
	}
	if (keys[MOVE_BACKWARD]) {
		pcamera->move(0, 0, -MOVE_SPEED * deltaTime);
	}
	if (keys[MOVE_LEFT]) {
		pcamera->move(-MOVE_SPEED * deltaTime, 0, 0);
	}
	if (keys[MOVE_RIGHT]) {
		pcamera->move(MOVE_SPEED * deltaTime, 0, 0);
	}
	if (keys[MOVE_UP]) {
		pcamera->move(0, MOVE_SPEED * deltaTime, 0);
	}
	if (keys[MOVE_DOWN]) {
		pcamera->move(0, -MOVE_SPEED * deltaTime, 0);
	}
}

void cameraRotateEvent(PiE::EngineContext &ctx, SDL_Event event) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (mouseLock) {
		pcamera->rotate(event.motion.xrel * 0.3f, event.motion.yrel * 0.3f);
	}
}

void mouseLockEvent(PiE::EngineContext & ctx, SDL_Event event) {
	switch (event.key.keysym.scancode) {
	case MOUSE_LOCK:
		if (!event.key.repeat) {
			SDL_SetRelativeMouseMode((SDL_bool)!mouseLock);
			mouseLock = !mouseLock;
		}
	}
}

int main(int argc, char** args) {
	//-------create context and initialize engine---------
	PiE::EngineContext ctx{};

	PiE::initEngine(ctx);

	SDL_SetWindowTitle(ctx.mainWindow, "Game Objects");
	//----------set up main camera and shader-------------
	Camera5DoF camera{ ctx.mainWindow };

	ctx.mainCamera = pcamera = &camera;

	shader.buildShader("vertexshader.txt", "unlitFragmentShader.txt");

	ctx.mainShader = &shader;
	//---------------add in game objects-----------------
	GameObject cubeObject{};
	ObjComponent cubeComponent{ "uv_cube.obj", shader };
	cubeObject.components.push_back(&cubeComponent);
	PiE::addGameObject(ctx, cubeObject);
	//----------add update and event functions-----------

	ctx.fixedUpdate.push_back(PiE::FixedUpdateCallback(cameraMoveCallback));

	ctx.events.insert({ SDL_EventType::SDL_MOUSEMOTION, PiE::EventCallback(cameraRotateEvent) });
	ctx.events.insert({ SDL_EventType::SDL_KEYDOWN, PiE::EventCallback(mouseLockEvent) });
	ctx.events.insert({ SDL_EventType::SDL_KEYDOWN, PiE::EventCallback(addRemoveGameObject) });

	//-------------------run main loop-------------------
	camera.move(0, 0, -3);
	PiE::startMainGameLoop(ctx, true);

	//-------------------game has quit-------------------
	for (GameObject *cube : cubes) {
		PiE::removeGameObject(ctx, *cube);
		for (Component * comp : cube->components) {
			delete(comp);
		}
		delete(cube);
	}
	cubes.clear();

	return 0;
}