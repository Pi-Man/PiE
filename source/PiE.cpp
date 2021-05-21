

#include "PiE.h"

namespace PiE {

	int initSDL(EngineContext &engineContext);

	int initEngine(EngineContext &engineContext) {

		if (initSDL(engineContext)) {
			printf("ERROR: Failed to initialize SDL2: %s\n", SDL_GetError());
			return -1;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			printf("ERROR: Failed to initialize SDL_IMAGE: %s\n", IMG_GetError());
			return -1;
		}

		glewExperimental = GL_TRUE;
		GLenum error = glewInit();
		if (error) {
			std::cout << "ERROR: " << glewGetErrorString(error) << std::endl;
			return -1;
		}

		OBJ::blankMtl.texture = new GL_Texture2D(10, 10, GL_NEAREST);
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				(*OBJ::blankMtl.texture)[i][j].get() = COLOR_A(255, 255, 255, 255);
			}
		}
		OBJ::blankMtl.texture->bindData();

		return 0;
	}

	int shutdownEngine(EngineContext & ctx) {
		ctx.running = false;
		Microphone::stop();
		Speaker::stop();
		SDL_DestroyWindow(ctx.mainWindow);
		SDL_Quit();
		return 0;
	}

	int addMesh(EngineContext &ctx, RenderObject &renderObject) {

		ctx.renderObjects.push_back(&renderObject);
		renderObject.transform.prev = &renderObject.prevTransform;
		renderObject.VAO.bindBuffers();

		return 0;
	}

	int addGameObject(EngineContext & ctx, GameObject & gameObject) {

		for (Component *component : gameObject.components) {
			component->gameObject = &gameObject;
			ctx.fixedUpdate.push_back([component](EngineContext &ctx) {component->fixedUpdate(ctx); });
			for (std::pair<Uint32, std::function<void(EngineContext &ctx, SDL_Event)>> & event : component->events) {
				ctx.events.insert(event);
			}
			component->onAdded(ctx, gameObject);
		}

		for (RenderObject *obj : gameObject.renderObjects) {
			addMesh(ctx, *obj);
		}

		gameObject.transform.prev = &gameObject.prevTransform;
		ctx.gameObjectTransforms.push_back(&gameObject.transform);

		return 0;
	}

	double getRenderPartialTick(EngineContext & ctx) {

		std::pair<double, double> times = ctx.tickLimiter.getLastNextTimes();
		double currentTime = (double)SDL_GetTicks();

		double pt = (currentTime - times.first) / ctx.tickLimiter.getLastFrameTime();
		pt = min(max(pt, 0.0), 1.0);

		return pt;
	}

	void setRenderContext(EngineContext & ctx, const RenderContext & renderContext) {

		if (renderContext.depthBuffer) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}

		if (renderContext.cullEnable) {
			glEnable(GL_CULL_FACE);
			glCullFace(renderContext.cullFace);
		}
		else {
			glDisable(GL_CULL_FACE);
		}

		if (renderContext.blendEnable) {
			glEnable(GL_BLEND);
			glBlendFunc(renderContext.blendSRC, renderContext.blendDST);
		}
		else {
			glDisable(GL_BLEND);
		}

		glClearColor(SPLIT_COLOR_F_A(renderContext.clearColor));
		glClearDepth(renderContext.clearDepth);

		if (renderContext.texture) {
			renderContext.texture->use();
		}
		else {
			OBJ::blankMtl.texture->use();
		}

		ctx.lastRenderContext = renderContext;

	}

	void render(EngineContext &ctx, Camera *camera, Shader shader) {

		std::lock_guard<std::mutex> lock(ctx.loopMutex);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Uint32 start = SDL_GetTicks();

		int w, h;

		SDL_GetWindowSize(ctx.mainWindow, &w, &h);

		glViewport(0, 0, w, h);

		Uint32 ms = SDL_GetTicks();

		Matrix4f transformation, prevTransformation;

		transformation = camera->getProjectionMatrix() * camera->getViewMatrix();
		prevTransformation = camera->getPrevProjectionMatrix() * camera->getPrevViewMatrix();

		double pt = getRenderPartialTick(ctx);

		if (ctx.lerp) transformation = Matrix4f::lerp(prevTransformation, transformation, pt);

		for (RenderObject *renderObject : ctx.renderObjects) {

			if (ctx.lastRenderContext != renderObject->renderContext) {
				setRenderContext(ctx, renderObject->renderContext);
			}

			glVertexAttrib4f(4, 1, 1, 1, 1);

			Shader _shader = shader;

			if (renderObject->shader.ID != -1) {
				shader = renderObject->shader;
			}

			GL_ERROR(glUseProgram(_shader.ID));

			GLint viewportID = glGetUniformLocation(_shader.ID, "viewport");
			GL_ERROR(glUniform2i(viewportID, w, h));

			GLint timeID = glGetUniformLocation(_shader.ID, "time");
			glUniform1f(timeID, ms / 1000.0f);

			for (Uniform& uniform : shader.uniforms) {
				uniform.apply();
			}

			if (ctx.dirLights.size() > 0) {

				std::vector<DirectionalLight*> dirLights = ctx.dirLights;
				std::sort(dirLights.begin(), dirLights.end(), [](DirectionalLight* light1, DirectionalLight* light2) {
					return light1->intensity >= light2->intensity;
				});

				GLfloat *dirData = (GLfloat*)malloc(sizeof(GLfloat) * ctx.dirLights.size() * 3);
				GLfloat *colData = (GLfloat*)malloc(sizeof(GLfloat) * ctx.dirLights.size() * 4);
				for (int i = 0; i < ctx.dirLights.size() && i < 4; i++) {

					Vec3f dir = ctx.lerp ? Vec3f::lerp(dirLights[i]->prevDir, dirLights[i]->dir, pt) : dirLights[i]->dir;
					dir /= dir();
					Vec3f col = ctx.lerp ? Vec3f::lerp(dirLights[i]->prevCol, dirLights[i]->col, pt) : dirLights[i]->col;
					float intensity = ctx.lerp ? Utils::lerp(dirLights[i]->prevIntensity, dirLights[i]->intensity, (float)pt) : dirLights[i]->intensity;

					dirData[i * 3 + 0] = dir[0];
					dirData[i * 3 + 1] = dir[1];
					dirData[i * 3 + 2] = dir[2];

					colData[i * 4 + 0] = col[0];
					colData[i * 4 + 1] = col[1];
					colData[i * 4 + 2] = col[2];
					colData[i * 4 + 3] = intensity;
				}
				GLint lightDirectionID = glGetUniformLocation(_shader.ID, "lightDirection");
				GLint dirLightColorID = glGetUniformLocation(_shader.ID, "dirLightColor");
				GLint directionLightCountID = glGetUniformLocation(_shader.ID, "directionLightCount");
				glUniform3fv(lightDirectionID, min(ctx.dirLights.size(), 4), dirData);
				glUniform4fv(dirLightColorID, min(ctx.dirLights.size(), 4), colData);

				glUniform1i(directionLightCountID, min(ctx.dirLights.size(), 4));
				free(dirData);
				free(colData);
			}

			if (ctx.pointLights.size() > 0) {

				std::vector<PointLight*> pointLights = ctx.pointLights;
				std::sort(pointLights.begin(), pointLights.end(), [renderObject](PointLight* light1, PointLight* light2) {
					return 1 / (light1->pos - renderObject->transform.globalPos())(2) * light1->intensity >= 1 / (light2->pos - renderObject->transform.globalPos())(2) * light2->intensity;
				});

				GLfloat *posData = (GLfloat*)malloc(sizeof(GLfloat) * ctx.pointLights.size() * 3);
				GLfloat *colData = (GLfloat*)malloc(sizeof(GLfloat) * ctx.pointLights.size() * 4);
				for (int i = 0; i < ctx.pointLights.size() && i < 4; i++) {

					Vec3f pos = ctx.lerp ? Vec3f::lerp(pointLights[i]->prevPos, pointLights[i]->pos, pt) : pointLights[i]->pos;
					Vec3f col = ctx.lerp ? Vec3f::lerp(pointLights[i]->prevCol, pointLights[i]->col, pt) : pointLights[i]->col;
					float intensity = ctx.lerp ? Utils::lerp(pointLights[i]->prevIntensity, pointLights[i]->intensity, (float)pt) : pointLights[i]->intensity;

					posData[i * 3 + 0] = pos[0];
					posData[i * 3 + 1] = pos[1];
					posData[i * 3 + 2] = pos[2];

					colData[i * 4 + 0] = col[0];
					colData[i * 4 + 1] = col[1];
					colData[i * 4 + 2] = col[2];
					colData[i * 4 + 3] = intensity;
				}
				GLint lightPositionID = glGetUniformLocation(_shader.ID, "lightPosition");
				GLint posLightColorID = glGetUniformLocation(_shader.ID, "posLightColor");
				GLint positionLightCountID = glGetUniformLocation(_shader.ID, "positionLightCount");
				glUniform3fv(lightPositionID, min(ctx.pointLights.size(), 4), posData);
				glUniform4fv(posLightColorID, max(ctx.pointLights.size(), 4), colData);

				glUniform1i(positionLightCountID, min(ctx.pointLights.size(), 4));
				free(posData);
				free(colData);
			}

			Matrix4f worldMatrix = ctx.lerp ? Matrix4f::lerp(renderObject->prevTransform, renderObject->transform, pt) : renderObject->transform;

			GLint transformationID = glGetUniformLocation(_shader.ID, "transformation");
			glUniformMatrix4fv(transformationID, 1, GL_FALSE, Matrix4f::transpose(transformation * worldMatrix));
			GLint worldMatrixID = glGetUniformLocation(_shader.ID, "worldMatrix");
			glUniformMatrix4fv(worldMatrixID, 1, GL_FALSE, Matrix4f::transpose(worldMatrix));

			renderObject->VAO.render();
		}

		SDL_GL_SwapWindow(ctx.mainWindow);

		Uint32 end = SDL_GetTicks();
	}

	std::thread startRenderThread(EngineContext & ctx) {

		SDL_GL_MakeCurrent(NULL, NULL);

		std::thread renderThread(startRenderLoop, std::ref(ctx));
		renderThread.detach();
		return renderThread;
	}

	void startRenderLoop(EngineContext & ctx) {

		SDL_GL_MakeCurrent(ctx.mainWindow, ctx.glContext);

		ctx.running = true;

		ctx.renderLimiter.start();

		setRenderContext(ctx, ctx.lastRenderContext);

		while (ctx.running) {
			ctx.renderLimiter.push();
			if (!(SDL_GetWindowFlags(ctx.mainWindow) & (SDL_WINDOW_MINIMIZED))) {
				ctx.renderer(ctx, ctx.mainCamera, ctx.mainShader);
			}
		}

	}

	void startMainGameLoop(EngineContext & ctx, const bool render) {

		ctx.running = true;

		ctx.tickLimiter.start();

		if (render) ctx.renderLimiter.start();

		while (ctx.running) {

			if (render) {
				double time = min(ctx.tickLimiter.getLastNextTimes().second, ctx.renderLimiter.getLastNextTimes().second);
				double sleep = time / 1000.0 - (double)SDL_GetTicks();
				if (sleep > 10.0) {
					Sleep(sleep);
				}
			}

			if (render && ctx.renderLimiter.poll().second) ctx.renderer(ctx, ctx.mainCamera, ctx.mainShader);

			if (!render) ctx.tickLimiter.push();

			if (!render || ctx.tickLimiter.poll().second) {

				if (ctx.lerp) {
					ctx.mainCamera->savePreviousState();

					for (RenderObject *obj : ctx.renderObjects) {
						*obj->transform.prev = obj->transform;
						if (obj->transform.parent) {
							obj->transform.prev->parent = obj->transform.parent->prev;
						}
					}

					for (Transform *transform : ctx.gameObjectTransforms) {
						*transform->prev = *transform;
						if (transform->parent) {
							transform->prev->parent = transform->parent->prev;
						}
					}

					for (DirectionalLight *light : ctx.dirLights) {
						light->prevDir = light->dir;
						light->prevCol = light->col;
						light->prevIntensity = light->intensity;
					}

					for (PointLight *light : ctx.pointLights) {
						light->prevPos = light->pos;
						light->prevCol = light->col;
						light->prevIntensity = light->intensity;
					}
				}

				SDL_Event event;

				while (SDL_PollEvent(&event)) {
					auto events = ctx.events.equal_range(event.type);
					for (auto it = events.first; it != events.second; it++) {
						it->second(ctx, event);
					}
				}

				for (std::function<void(EngineContext&)> &f : ctx.fixedUpdate) {
					f(ctx);
				}
			}
		}
	}









	int initSDL(EngineContext &ctx) {
		if (SDL_Init(SDL_INIT_VIDEO)) {
			printf("ERROR: initializing SDL2: %s\n", SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ctx.OPENGL_MAJOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ctx.OPENGL_MINOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if (ctx.MSAA > 0) {
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ctx.MSAA);
		}

		ctx.mainWindow = SDL_CreateWindow("PiE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ctx.windowSize.x, ctx.windowSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		if (ctx.mainWindow == nullptr) {
			std::cout << "ERROR: " << SDL_GetError() << std::endl;
			return -1;
		}

		ctx.glContext = SDL_GL_CreateContext(ctx.mainWindow);

		if (ctx.glContext == NULL) {
			std::cout << "ERROR: " << SDL_GetError() << std::endl;
			return -1;
		}

		SDL_GL_MakeCurrent(ctx.mainWindow, ctx.glContext);

		return 0;
	}

}