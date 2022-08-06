#include <Windows.h>

#include <PiE.h>
#include <Clickable.h>
#include <TexturedRect.h>
#include <VertexAttribute.h>
#include <Tessellator.h>
#include <FontRenderer.h>
#include <TextElement.h>

#include <Optional.h>

Camera *camera;

VertexAttributeType colorAttribute(Usage::OTHER, Attribute().make<Attribute_Type::VEC3>({ 1, 0.5, 0.25 }));

int main(int args, char **argv) {

	PiE::EngineContext ctx{ 30, 30, 3, 3, 4 };

	PiE::initEngine(ctx);

	Camera5DoF _camera(ctx.mainWindow);
	Camera5DoF canvas_camera(ctx.mainWindow);
	canvas_camera.orthogonal = true;
	canvas_camera.setClipPlanes(-10, 10);
	canvas_camera.FOV = 1;

	camera = &_camera;

	Shader shader;
	shader.buildShader("vertexShader.txt", "unlitFragmentShader.txt");

	ctx.mainCamera = camera;
	ctx.mainShader = &shader;

	Shader textShader2;
	textShader2.buildShader("vertexShader.txt", "text_fragment_shader.glsl");



	const char *fontFile = "DSEG7Classic-Regular.ttf";
	//const char *fontFile = "Sun Flower.ttf";
	Font fontObject(fontFile);

	AxisAlignedRect bounds{};
	bounds.ctx = &ctx;
	bounds.center = { {0.5_ndc, 0.5_ndc} };
	bounds.width = 0.1_vw;
	bounds.height = 1.0_vh;

	GL_Texture2D testImage("test.png");
	GL_Texture2D buttonImage("addButton.png");

	GameObject canvas{};
	TexturedRect buttonTexture{ &bounds, &buttonImage, camera };
	TextElement buttonText(&bounds, &textShader2, fontObject, "3.1415", true, true);

	srand(0);
	Clickable buttonClick{ &bounds };
	buttonClick.onRelease = PiE::EventCallback([&bounds, &buttonTexture, &testImage, &buttonImage, &fontObject, &buttonText, &buttonClick](PiE::EngineContext &ctx, SDL_Event event) {
		if (buttonClick.hovered && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 1) {
			bounds.center = { {AxisAlignedRect::Value{rand() / (double)RAND_MAX, AxisAlignedRect::Value::NDC}, AxisAlignedRect::Value{rand() / (double)RAND_MAX, AxisAlignedRect::Value::NDC}} };
			bounds.width = 0.2_vw;
			bounds.height = 0.1_vw;
			bounds.reset(bounds.center, bounds.width, bounds.height);
			buttonTexture.m.invalidate();
			buttonText.m.invalidate();
		}
	});
	buttonClick.onHoverStart = PiE::EventCallback([&buttonTexture, &testImage](PiE::EngineContext & ctx, SDL_Event event) {
		buttonTexture.texture = &testImage;
	});
	buttonClick.onHoverEnd = PiE::EventCallback([&buttonTexture, &buttonImage](PiE::EngineContext & ctx, SDL_Event event) {
		buttonTexture.texture = &buttonImage;
	});
	canvas.components.push_back(&buttonClick);
	canvas.components.push_back(&buttonTexture);
	buttonTexture.renderObject.renderContext.camera = &canvas_camera;
	canvas.components.push_back(&buttonText);
	buttonText.textObject.renderContext.camera = &canvas_camera;
	PiE::addGameObject(ctx, canvas);

	PiE::startMainGameLoop(ctx, true);

	return 0;
}